/*
 * Copyright 2012 Anthony Cassidy
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#include "FolderTraverser.h"
#include "A2GASSERT.h"
#include "FOLDERSONLY.h"
#include <time.h>
#include <locale>
#include <QRgb>
#include <QImage>

#include <QImage>
#include <QRect>
#include <QSet>
#include <QList>
#include <QDomDocument>
#include <QTextStream>
#include <QTextBrowser>
#include <QVariant>
#include "OFile.h"
#include "IFile.h"
#include "LoaderAndResFilePair.h"
#include "AFile.h"
#include "GetObjectPlusAnimHash.h"
//#include "SaveDelayLoadRes.h"


using namespace com::github::a2g::generator;

const char* _00_ANIMATIONS = "_00_Animations";


void FolderTraverser::generate(QString rootFolder, QString package)
{
    QString bad = "?"; //this is a char that will invalidate a filename, and thus any XFile class if used as param1

    QString animFolder = _00_ANIMATIONS;
    AFile aFile(rootFolder, animFolder, package);

    QStringList foldersOfPsdFile = files.getSubFolders(rootFolder);
    for(QStringList::iterator psdFileFolder=foldersOfPsdFile.begin();psdFileFolder!=foldersOfPsdFile.end();psdFileFolder++)
    {
        QString psdFileSeg = QDir(*psdFileFolder).dirName();
        if(psdFileSeg == "_00_Animations")
            continue;
        bool isInAnObjectsFolder = psdFileSeg.contains("_Main");
        bool isInAnInventoryFolder = psdFileSeg.contains("_Inventory");

        OFile oStream(isInAnObjectsFolder? *psdFileFolder : bad, psdFileSeg, package);
        IFile iStream(isInAnInventoryFolder? *psdFileFolder: bad, psdFileSeg, package);

        QStringList pixelFolders = files.getSubFolders(*psdFileFolder);
        for(QStringList::iterator pixelFolder=pixelFolders.begin();pixelFolder!=pixelFolders.end();pixelFolder++)
        {			
            QString pixelSeg = QDir(*pixelFolder).dirName();


            LoaderAndResFilePair resStream( *psdFileFolder, pixelSeg, psdFileSeg, isInAnInventoryFolder? NULL: animFolder, package);
            LoaderAndResFilePair initialStream( *psdFileFolder, pixelSeg, psdFileSeg, isInAnInventoryFolder? NULL: animFolder, package);

            std::list<QString> objectFolders = files.getSubFolders(*pixelFolder).toStdList();
            for(std::list<QString>::reverse_iterator objectFolder=objectFolders.rbegin();objectFolder!=objectFolders.rend();objectFolder++)
            {
                QString objectSeg = QDir(*objectFolder).dirName();

                if(isInAnInventoryFolder)
                {
                    QStringList PNGs = files.getSubFiles(*objectFolder);
                    for(int i=0;i<PNGs.size();i++)
                    {
                        QString pngPath = PNGs[i];
                        if(pngPath.right(4) != ".png")
                            continue;
                        QString invSeg = objectSeg;
                        int idForInv = iStream.getIdForName(invSeg);

                        resStream.addInvImage(pngPath, idForInv);

                        break;//only take the first png in each folder
                    }
                }
                else
                {
                    int idForObj = oStream.getIdForName(getRealObjectSeg(objectSeg));

                    QStringList animFolders = files.getSubFolders(*objectFolder);
                    for(QStringList::iterator animFolder = animFolders.begin();animFolder!=animFolders.end();animFolder++)
                    {
                        QString animSeg = QDir(*animFolder).dirName().toLower();

                        int idForObjPlusAnim = getObjectPlusAnimHash(objectSeg,animSeg);

                        aFile.insert(getObjectPlusAnim(objectSeg,animSeg).toStdString(), idForObjPlusAnim);
                        QStringList PNGs = files.getSubFiles(*animFolder);

                        for(QStringList::iterator pngLoadMe = PNGs.begin();pngLoadMe!=PNGs.end();pngLoadMe++)
                        {   
                            if(pngLoadMe->right(4) != ".png")
                                continue;

                            if(isInAnObjectsFolder && (animSeg=="initial" || animSeg=="blank"))
                            {
                                initialStream.addAnimImage(*pngLoadMe, idForObj);
                            }
                            else
                            {
                                resStream.addAnimImage(*pngLoadMe, isInAnObjectsFolder? idForObj : -1);//if the id is -1 then it signals the engine to look for an existing object to add the anim image to
                            }
                        }

                    }
                }
            }

            // set the names in one place;
            // if both the streams have content, and thus will write a valid file, then "first"+"last"
            // but if either is empty, and thus only one file then "only"
            if(initialStream.isEmpty() || resStream.isEmpty())
            {
                initialStream.setJavaClassNamePrefix("Only");
                resStream.setJavaClassNamePrefix("Only");
                output.addOnly(initialStream);
                output.addOnly(resStream);
            }
            else
            {
                initialStream.setJavaClassNamePrefix("First");
                resStream.setJavaClassNamePrefix("Last");
                output.addFirst(initialStream);
                output.addLast(resStream);
            }


        }
        if(iStream.getNumberOfEntries()>0)
        {
            output.addIFile(iStream);
        }
        if(oStream.getNumberOfEntries()>0)
        {
            output.addOFile(oStream);
        }
    }

    if(aFile.getNumberOfEntries()>0)
    {
        output.addAFile(aFile);
    }
}	

void FolderTraverser::searchForAllSubFoldersContainingKeyFolderAndGenerateIfFound(QString startingPath, QString targetPathSegment)
{
    startingPath.replace("\\","/");
    return searchRecursively(startingPath, targetPathSegment);
}

void FolderTraverser::searchRecursively(QString folder, QString targetPathSegment)
{
    QStringList subFolders = files.getSubFolders(folder);
    for(int i=0;i<subFolders.count();i++)
    {
        QString subFolder = subFolders.at(i);
        if(subFolder.contains(_00_ANIMATIONS))
            continue;//


        QString sourceFolderSeg = "src";


        int startOfSRC = subFolder.lastIndexOf(sourceFolderSeg);
        int end = subFolder.lastIndexOf(targetPathSegment);
        if(startOfSRC!=-1 && end!=-1 && startOfSRC<=end)
        {
            // we've found the target level, as prescribed by the start and end folders
            QStringList subsubFolders = files.getSubFolders(subFolder);
            for(int j=0;j<subsubFolders.count();j++)
            {
                QString subsubFolder = subsubFolders.at(j);
                if(subsubFolder.contains(_00_ANIMATIONS))
                    continue;// ..or else we keep generating animations folders in animations folders..

                QString folderSeg = QDir(subsubFolder).dirName();
                int offset = sourceFolderSeg.length() + 1; // src.com, the '+1' is to skip past the dot
                int startOfTLD = startOfSRC + offset;//       ^   ^
                QString package = subsubFolder.mid(startOfTLD,end+targetPathSegment.length()+folderSeg.length()+1 );
                package.replace('/','.');
                generate(subsubFolder, package);
            }
            // since we've processed all the subfolders at our target, then we return
            // otherwise it will keep recursing through subdirs, and these will keep matching..
            // ...since they contain both the start and the end.
            // and then we'll get Generate called in many places, and _00_Animations folders in many places.
            return;
        }
        //recurse
        searchRecursively(subFolder,  targetPathSegment);
    }
}


