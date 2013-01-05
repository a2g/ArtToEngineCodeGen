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
#include "SOURCEIM.h"
#include "SRC.h"


using namespace com::github::a2g::generator;

const char* _00_ANIMATIONS = "_00_animations";


void FolderTraverser::generate(QString rootFolder, QString package)
{
    QString bad = "?"; //this is a char that will invalidate a filename, and thus any XFile class if used as param1

    QString animFolder = _00_ANIMATIONS;
    AFile aFile(rootFolder, animFolder, package);

    QStringList foldersOfPsdFile = files.getSubFolders(rootFolder);
    for(QStringList::iterator psdFileFolder=foldersOfPsdFile.begin();psdFileFolder!=foldersOfPsdFile.end();psdFileFolder++)
    {
        QString psdFileSeg = QDir(*psdFileFolder).dirName();
        if(psdFileSeg == _00_ANIMATIONS)
            continue;
        bool isInAnObjectsFolder = psdFileSeg.contains("_main");
        bool isInAnInventoryFolder = psdFileSeg.contains("_inventory");

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

void FolderTraverser::generateFilesFromSourceFolderOrASubFolderThereof(QString path)
{
    path.replace("\\","/");

    // get dirs of child and parent
    QDir dir(path);
    int j = path.lastIndexOf(dir.dirName());
    path.truncate(j-1);
    QDir parentDir(path);

    // get names of dir and parent
    QString dirName = dir.dirName().toUpper();
    QString parentDirName = parentDir.dirName().toUpper();

    if(dirName == SOURCEIM)
    {
        processAllSubFolders(dir.absolutePath());
    }
    else if (parentDirName == SOURCEIM)
    {
        processJustThisSubFolder(dir.absolutePath());
    }
}

void FolderTraverser::processAllSubFolders(QString folder)
{
    QStringList subFolders = files.getSubFolders(folder);
    for(int i=0;i<subFolders.count();i++)
    {
        QString subFolder = subFolders.at(i);
        if(subFolder.contains(_00_ANIMATIONS))
            continue;//
        processJustThisSubFolder(subFolder);
    }
}



void FolderTraverser::processJustThisSubFolder(QString subFolder)
{
    subFolder = subFolder.toLower();
    int findSRC = subFolder.lastIndexOf(QString(SRC).toLower());
    if(findSRC!=-1)
    {
        int offset = QString(SRC).length() + 1; // src.com, the '+1' is to skip past the dot
        int startOfCOM = findSRC + offset;//       ^   ^
        QString package = subFolder.mid(startOfCOM);
        package.replace('/','.');
        generate(subFolder, package);
    }
}


