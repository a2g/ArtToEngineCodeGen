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

#include <QMap>
#include <QDir>
#include <DomLocation.h>
#include <crop.h>
#include "getObjectPlusanim.h" 
#include "allcaps\SOURCEIMAGES.h"
namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                static void cropAllFramesAndUpdateImagePaths(DomLocation& l)
                {
                    QMap<QString,DomAnimation> theSet;
                    for(int g = 0; g < l.getCameraCount(); g++)
                    {
                        auto c = l.getCameraAt(g);
                        for(int h = 0; h < c.getObjectCount(); h++)
                        {
                            auto o = c.getObjectAt(h);
                            for(int i = 0; i < o.getAnimationCount(); i++)
                            {
                                auto a = o.getAnimationAt(i);
                                {

                                    for(int j = 0; j < a.getFrames().size(); j++)
                                    {
                                        const DomFrame& f = a.getFrames().at(j);
                                        std::string debug = f.getPngSaveTo();
                                        QStringList list = QString(f.getPngSaveTo()).split("/");
                                        QString root;
                                        int i = 0;
                                        for(; i < list.count() - 1; i++)
                                        {
                                            root += list[i] + "/";
                                        }

                                        std::string debug2 = root.toUtf8().data();
                                        QString locationFolder = QDir(root).absolutePath();
                                        std::string debug3 = locationFolder.toUtf8().data();
                                        QString cameraSeg = list[i - 4];
                                        QString pixelSeg = list[i - 3];
                                        QString objectSeg = list[i - 2];
                                        QString animSeg = list[i - 1];

                                        QString objectPlusAnim = getObjectPlusAnim(objectSeg,animSeg);



                                        QString pngPath = f.getPngSaveTo();

                                        QString pngByItself = "cropped_" + QDir(pngPath).dirName();
                                        QString pngSaveTo = locationFolder + "/" + pngByItself;

                                        pngSaveTo = pngSaveTo.replace(SOURCEIMAGES.toLower(),VISUALS.toLower());

                                        // do the cropping
                                        QRect rect;
                                        if(!cameraSeg.toUpper().contains("INV"))
                                        {
                                           rect = crop(pngPath,pngSaveTo,Qt::red);
                                        }
                                        else
                                        {
                                            pngSaveTo.replace("cropped_","");
                                            QFile file(pngPath);
                                            file.copy(pngSaveTo);
                                        }
                                        DomFrame& writeable = const_cast<DomFrame&>(f);
                                        writeable.setRect(rect);
                                        std::string debug10 = pngSaveTo.toUtf8().data();

                                        writeable.setPngPath(pngSaveTo.toUtf8().data());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

