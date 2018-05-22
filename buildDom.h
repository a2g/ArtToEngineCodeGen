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

#pragma once

#include <QFile>
#include <QVector>
#include <FileSystem.h>
#include <DomLocation.h>
#include "allcaps\NAMESPACE_BEGIN.h"
#include "allcaps\NAMESPACE_END.h"
#include "allcaps\AFILE.h"
#include <getPackageFromDosPathContainingSrc.h>
#include "isShared.h"
#include "isInventory.h"
#include "isPngOrBmp.h"
#include "getObjectPlusAnimHash.h"
#include "getRealObjectSeg.h"
#include "getObjectPlusAnim.h"
#include "getObjectPlusAnimPlusNumber.h"
#include "isInventory.h"

NAMESPACE_BEGIN
static std::auto_ptr<DomLocation> buildDom(const QString& dosPathOfLocation,const FileSystem& fileSystem)
{
    QString bad = "?"; //this is a char that will invalidate a filename, and thus any XFile class if used as param1

    std::auto_ptr<DomLocation> toReturn;
    toReturn.reset(new DomLocation());
    DomLocation& domLocation = toReturn->asRef();
    domLocation.locationSeg = QDir(dosPathOfLocation).dirName();
    domLocation.locationPath = dosPathOfLocation.toUpper();
    domLocation.fullLocationPackage = getPackageFromDosPathContainingSrc(dosPathOfLocation);

    QStringList camerasInLocation = fileSystem.getSubFolders(dosPathOfLocation);
    for(QStringList::iterator cameraPath = camerasInLocation.begin(); cameraPath != camerasInLocation.end(); cameraPath++)
    {
        QString cameraSeg = QDir(*cameraPath).dirName();
        if(cameraSeg == AFILE)
            continue;

        DomCamera& domCamera = domLocation.addNewCamera(cameraSeg);

        std::list<QString> objectsOfCamera = fileSystem.getSubFolders(*cameraPath).toStdList();
        for(std::list<QString>::reverse_iterator objectPath = objectsOfCamera.rbegin(); objectPath != objectsOfCamera.rend(); objectPath++)
        {
            QString objectSeg = QDir(*objectPath).dirName();

            DomObject& domObject = domCamera.addNewObject(objectSeg);

            if(isInventory(cameraSeg))
            {

                DomAnimation& theAnimation = domObject.addNewAnimation();
                theAnimation.animSeg = "inv";

                QStringList pathsToPngs = fileSystem.getSubFiles(*objectPath);
                for(int i = 0; i<pathsToPngs.size(); i++)
                {
                    QString pathToPng = pathsToPngs[i];
                    if(!isPngOrBmp(pathToPng))
                        continue;
                    QString invSeg = objectSeg;
                    std::string debug = pathToPng.toUtf8().data();
                    theAnimation.addFrame(pathToPng.toUtf8().data());

                    break;//only take the first png in each folder
                }

            }
            else
            {
                domObject.prefix = objectSeg.mid(1,2).toInt();
                QStringList animsOfObject = fileSystem.getSubFolders(*objectPath);
                for(QStringList::iterator animPath = animsOfObject.begin(); animPath != animsOfObject.end(); animPath++)
                {
                    QString animSeg = QDir(*animPath).dirName().toLower();

                    if(animSeg == "ignore")
                        continue;

                    DomAnimation& animation = domObject.addNewAnimation(animSeg);

                    QStringList pathsToPngs = fileSystem.getSubFiles(*animPath);

                    int number = 0;
                    for(QStringList::iterator pathToPng = pathsToPngs.begin(); pathToPng != pathsToPngs.end(); pathToPng++,number++)
                    {
                        if(!isPngOrBmp(*pathToPng))
                            continue;

                        animation.addFrame(pathToPng->toUtf8().data());
                    }
                }
            }
        }
    }
    return toReturn;
}
NAMESPACE_END