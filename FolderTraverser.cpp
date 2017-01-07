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
#include <time.h>
#include <locale>
#include <QtGui\QRgb>
#include <QtGui\QImage>

#include <QtGui\QImage>
#include <QRect>
#include <QSet>
#include <QList>
#include <QtXml\QDomDocument>
#include <QTextStream>
#include <QtGui\QTextBrowser>
#include <QVariant>
#include "OFile.h"
#include "IFile.h"
#include "LoaderAndItsBundles.h"
#include "AFile.h"
#include "GetObjectPlusAnimHash.h"
#include "SOURCEIMAGES.h"
#include "SRC.h"
#include "IsPngOrBmp.h"
#include "IsInventory.h"
#include "IsShared.h"
#include "_00_ANIMATIONS.h"
using namespace com::github::a2g::generator;

FolderTraverser::FolderTraverser(FileSystem& fs, OutputFiles* output)
                        : files(fs)
                        , output(output)
                        , isOnlyOnly(false)
                    {
                        isOnlyOnly = 0;
                    }

void FolderTraverser::generate(QString rootFolder, QString package)
{
    QString bad = "?"; //this is a char that will invalidate a filename, and thus any XFile class if used as param1

    AFile aFile(rootFolder, package);

    QStringList foldersOfPsdFile = files.getSubFolders(rootFolder);
    for(QStringList::iterator psdFileFolder=foldersOfPsdFile.begin();psdFileFolder!=foldersOfPsdFile.end();psdFileFolder++)
    {
        QString psdFileSeg = QDir(*psdFileFolder).dirName();
        if(psdFileSeg == _00_ANIMATIONS)
            continue;
        bool isInAnObjectsFolder = !IsShared(psdFileSeg);
        bool isInAnInventoryFolder = IsInventory(psdFileSeg);

        OFile oStream(package, psdFileSeg, isInAnObjectsFolder? *psdFileFolder : bad );
        IFile iStream(package, psdFileSeg, isInAnInventoryFolder? *psdFileFolder: bad );

        LoaderAndItsBundles resStream(package, psdFileSeg, *psdFileFolder);
        LoaderAndItsBundles initialStream(package, psdFileSeg, *psdFileFolder);

        std::list<QString> objectFolders = files.getSubFolders(*psdFileFolder).toStdList();
        for(std::list<QString>::reverse_iterator objectFolder=objectFolders.rbegin();objectFolder!=objectFolders.rend();objectFolder++)
        {
            QString objectSeg = QDir(*objectFolder).dirName();

            if(isInAnInventoryFolder)
            {
                QStringList PNGs = files.getSubFiles(*objectFolder);
                for(int i=0;i<PNGs.size();i++)
                {
                    QString pngPath = PNGs[i];
                    if(!IsPngOrBmp(pngPath))
                        continue;
                    QString invSeg = objectSeg;
                    int idForInv = iStream.getIdForName(invSeg);

                    resStream.addInvImage(pngPath, idForInv);

                    break;//only take the first png in each folder
                }
            }
            else
            {
                int idForObj = oStream.getIdForName(getRealObjectSeg(objectSeg,false));

                QStringList animFolders = files.getSubFolders(*objectFolder);
                for(QStringList::iterator animFolder = animFolders.begin();animFolder!=animFolders.end();animFolder++)
                {
                    QString animSeg = QDir(*animFolder).dirName().toLower();

                    if(animSeg=="ignore")
                        continue;
                    int idForObjPlusAnim = getObjectPlusAnimHash(objectSeg,animSeg);

                    aFile.insert(getObjectPlusAnim(objectSeg,animSeg).toStdString(), idForObjPlusAnim);
                    QStringList PNGs = files.getSubFiles(*animFolder);

                    for(QStringList::iterator pngLoadMe = PNGs.begin();pngLoadMe!=PNGs.end();pngLoadMe++)
                    {
                        if(!IsPngOrBmp(*pngLoadMe))
                            continue;

                        // "blank" is usd when initial already exists, for that object, in some otherrender output
                        //
                        if(isInAnObjectsFolder && (animSeg=="initial" || animSeg=="blank"||animSeg=="placeholder"))
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


            // set the names in one place;
            // if both the streams have content, and thus will write a valid file, then "first"+"last"
            // but if either is empty, and thus only one file then "only"
            // 2017, decided that I'm using Only for everything.
            if(isOnlyOnly || initialStream.isEmpty() || resStream.isEmpty())
            {
                initialStream.setJavaClassNamePrefix("Only");
                resStream.setJavaClassNamePrefix("Only");
                if(output)
                {
                    output->addOnly(initialStream);
                    output->addOnly(resStream);
                }
            }
            else
            {
                initialStream.setJavaClassNamePrefix("First");
                resStream.setJavaClassNamePrefix("Last");
                if(output)
                {
                    output->addFirst(initialStream);
                    output->addLast(resStream);
                }
            }
        }
        if(iStream.getNumberOfEntries()>0)
        {
             if(output)
            {
                output->addIFile(iStream);
            }
        }
        if(oStream.getNumberOfEntries()>0)
        {
             if(output)
            {
                output->addOFile(oStream);
            }
        }
    }

    if(aFile.getNumberOfEntries()>0)
    {
        if(output)
        {
            output->addAFile(aFile);
        }
    }
}	

FolderTraverserResult getErrorFromInputPath(QString path)
{
    path = path.toUpper();
    if(!path.contains(SRC))
        return  SrcIsNotFoundInPathParameter;
    if(!path.contains(SOURCEIMAGES))
        return SourceImagesIsNotFoundInPathParameter;
    return IsOk;
}

FolderTraverserResult FolderTraverser::generateFilesFromSourceFolderOrASubFolderThereof(QString path)
{
    FolderTraverserResult error = getErrorFromInputPath(path);
    if(error > 1)
        return error;

    if(output)
        output->clear();

    path.replace("\\","/");


    // get dirs of child and parent
    QDir dir(path);
    int j = path.lastIndexOf(dir.dirName());
    path.truncate(j-1);
    QDir parentDir(path);

    // get names of dir and parent
    QString dirName = dir.dirName().toUpper();
    QString parentDirName = parentDir.dirName().toUpper();

    if(dirName == SOURCEIMAGES)
    {
        processAllSubFolders(dir.absolutePath());

    }
    else if (parentDirName == SOURCEIMAGES)
    {
        processJustThisSubFolder(dir.absolutePath());

    }
    return IsOk;
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

    int offset = QString(SRC).length() + 1; // src.com, the '+1' is to skip past the dot
    int startOfCOM = findSRC + offset;//       ^   ^
    QString package = subFolder.mid(startOfCOM);
    package.replace('/','.');
    generate(subFolder, package);
}


