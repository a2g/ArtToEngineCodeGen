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
#include <QDir>
#include <QFile>
#include <QImage>
#include <QMessageBox>
#include <QDateTime>
#include <QTime>
#include <QTextStream>
#include <QDomDocument>
#include <QXmlStreamWriter>
#include "allcaps\A2GASSERT.h"
#include <iostream>


const char* STR_IMAGE = "croppedImage";
const char* STR_ZBUFFER = "croppedZBuffer";

const char* YEAR = "year";
const char* MONTH = "month";
const char* DAY = "day";
const char* HOUR = "hour";
const char* MINUTE = "minute";
const char* SECOND = "second";
const char* MILLISECOND = "millisecond";

const char* TOP = "top";
const char* LEFT = "left";
const char* WIDTH = "width";
const char* HEIGHT = "height";

#include <QtGui\qrgb.h>
#include <QRect>
namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                QRect getBoundingNonBlackRectangle(QImage image)
                {
                    int lastCol = image.width()-1;
                    int lastRow = image.height()-1;


                    // back up corner pixels
                    QRgb bottomRightPixel = image.pixel(lastCol,lastRow);
                    QRgb topRightPixel = image.pixel(lastCol,0);
                    QRgb bottomLeftPixel = image.pixel(0,lastRow);
                    QRgb topLeftPixel = image.pixel(0,0);

                    // set corners pixels to black
                    // we need to set black in all the four corners because of createHeiristicMask
                    // which is used in GenerateZBufferRepresentation, this takes a color key from the four corners.
                    image.setPixel(lastCol,lastRow,0);
                    image.setPixel(lastCol,0,0);
                    image.setPixel(0,lastRow,0);
                    image.setPixel(0,0,0);

                    unsigned int max = image.width() > image.height()? image.width() : image.height();
                    unsigned int top = 0;
                    unsigned int left = 0;
                    unsigned int bottom = image.height()-1;
                    unsigned int right = image.width()-1;
                    bool isToKeepShrinkingTop = true;
                    bool isToKeepShrinkingLeft = true;
                    bool isToKeepShrinkingBottom = true;
                    bool isToKeepShrinkingRight = true;
                    // this turns off the optimization :return QRect(QPoint(left,top),QPoint(right,bottom));

                    // This is finding the bounding box by keeping shrinking the square (top,left,bottom,right)
                    // until it runs into some non zero pixels.
                    // we alternate between vertical and horizontal, because overall, this results in
                    // fewer pixels to scan ( every horizontal line scanned can be two pixels shorter than the previous)
                    for(unsigned int i=0;i<max;i++)
                    {
                        // one comparison per column is probably ok.
                        // top and left start off as zero, so if they go bigger than their counterpart then bail - image is blank!
                        if(top>=bottom||left>=right)
                            return QRect(0,0,0,0);
                        for(unsigned int x=left;x<=right && (isToKeepShrinkingTop || isToKeepShrinkingBottom);x++)
                        {
                            int colorTop = image.pixel(x,top);
                            int colorBottom = image.pixel(x,bottom);
                            // keep Shrinking when:
                            // - pixel is transparent (alpha = opacity) so if alpha == 0, then 0% opaque.
                            // - either the pixel is black (ie 3dsmax always renders a background color, and it
                            //   looks best if its black if we insist if its black)
                            isToKeepShrinkingTop &= (colorTop&RGB_MASK)==0  ||  ::qAlpha(colorTop)==0;
                            isToKeepShrinkingBottom &= (colorBottom&RGB_MASK)==0  ||  ::qAlpha(colorBottom)==0;
                            // we put the black test first, because > 90% will be max renders
                        }

                        top += isToKeepShrinkingTop;
                        bottom -= isToKeepShrinkingBottom;

                        for(unsigned int y=top;y<=bottom && (isToKeepShrinkingLeft || isToKeepShrinkingRight);y++)
                        {
                            int colorLeft = image.pixel(left,y);
                            int colorRight = image.pixel(right,y);
                            isToKeepShrinkingRight &= (colorRight&RGB_MASK)==0  ||  ::qAlpha(colorRight)==0;
                            isToKeepShrinkingLeft &= (colorLeft&RGB_MASK)==0  ||  ::qAlpha(colorLeft)==0;
                        }

                        left += isToKeepShrinkingLeft;
                        right -= isToKeepShrinkingRight;
                    }

                    QPoint topLeft(left,top);
                    QPoint bottomRight(right,bottom);

                    QRect toReturn(topLeft,bottomRight);

                    // put corners pixels back
                    image.setPixel(lastCol,lastRow,bottomRightPixel);
                    image.setPixel(lastCol,0,topRightPixel);
                    image.setPixel(0,lastRow,bottomLeftPixel);
                    image.setPixel(0,0,topLeftPixel);

                    return toReturn;
                }


                QImage generateZBufferRepresentation(QImage image,QRgb zbufferColor)
                {
                    QImage zbuf = image.createHeuristicMask(false);
                    zbuf.invertPixels();
                    QImage alphed(zbuf.width(),zbuf.height(),QImage::Format_ARGB32_Premultiplied);
                    alphed.fill(zbufferColor);// if aplha, then fill will overwrite alpha
                    alphed.setAlphaChannel(zbuf);
                    return alphed;
                }

                QDateTime getSaveTime(QString fullPath)
                {
                    QFile file(fullPath);
                    QFileInfo fileinfo(file);
                    QDateTime pngSaveTime = fileinfo.lastModified();
                    return pngSaveTime;
                }
                QString getXmlFileForPng(QString pngFile)
                {
                    return pngFile+".xml";
                }

                QRect getCachedRectIfValidOneExists(QString pngFile)
                {
                    QByteArray debug = pngFile.toUtf8();

                    QRect rect;
                    if(QFile::exists(getXmlFileForPng(pngFile)))
                    {
                        QFile file(getXmlFileForPng(pngFile));
                        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
                            return rect;

                        QDomDocument doc;
                        if(doc.setContent(&file))
                        {
                            QDomElement root = doc.documentElement();

                            int year = root.attribute(YEAR).toInt();
                            int month = root.attribute(MONTH).toInt();
                            int day = root.attribute(DAY).toInt();
                            QDate lastDate(year,month,day);

                            int h = root.attribute(HOUR).toInt();
                            int m = root.attribute(MINUTE).toInt();
                            int s = root.attribute(SECOND).toInt();
                            int ms = root.attribute(MILLISECOND).toInt();
                            QTime lastTime(h,m,s,ms);

                            // datetime from XML
                            QDateTime timeFromXml(lastDate,lastTime);

                            // datetime from actual png
                            QDateTime pngSaveTime = getSaveTime(pngFile);

                            // if the time matches thenit hasn't been modified since last time.
                            if(pngSaveTime.date().year()==timeFromXml.date().year())
                            {
                                if(pngSaveTime.date().month()==timeFromXml.date().month())
                                {
                                    if(pngSaveTime.date().day()==timeFromXml.date().day())
                                    {
                                        if(pngSaveTime.time().hour()==timeFromXml.time().hour())
                                        {
                                            if(pngSaveTime.time().minute()==timeFromXml.time().minute())
                                            {
                                                if(pngSaveTime.time().second()-timeFromXml.time().second()<30)
                                                {
                                                    int top = root.attribute(TOP).toInt();
                                                    int left = root.attribute(LEFT).toInt();
                                                    int width = root.attribute(WIDTH).toInt();
                                                    int height = root.attribute(HEIGHT).toInt();
                                                    rect.setTop(top);
                                                    rect.setLeft(left);
                                                    rect.setWidth(width);
                                                    rect.setHeight(height);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        file.close();
                    }
                    return rect;
                }

                void saveTimestampXml(QString pngFile,QRect rect)
                {
                    QDateTime pngSaveTime = getSaveTime(pngFile);



                    QString name = getXmlFileForPng(pngFile);
                    QFile outFile(name);
                    if(!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
                    {
                        qDebug("Failed to open file for writing.");
                        return;
                    }

                    {
                        QXmlStreamWriter stream(&outFile);
                        stream.setAutoFormatting(true);
                        stream.writeStartDocument();

                        stream.writeStartElement("root");
                        stream.writeAttribute("href","http://qt.nokia.com/");
                        stream.writeAttribute(YEAR,QString::number(pngSaveTime.date().year()));
                        stream.writeAttribute(MONTH,QString::number(pngSaveTime.date().month()));
                        stream.writeAttribute(DAY,QString::number(pngSaveTime.date().day()));
                        stream.writeAttribute(HOUR,QString::number(pngSaveTime.time().hour()));
                        stream.writeAttribute(MINUTE,QString::number(pngSaveTime.time().minute()));
                        stream.writeAttribute(SECOND,QString::number(pngSaveTime.time().second()));
                        stream.writeAttribute(MILLISECOND,QString::number(pngSaveTime.time().msec()));
                        stream.writeAttribute(TOP,QString::number(rect.top()));
                        stream.writeAttribute(LEFT,QString::number(rect.left()));
                        stream.writeAttribute(WIDTH,QString::number(rect.width()));
                        stream.writeAttribute(HEIGHT,QString::number(rect.height()));

                        stream.writeEndElement(); // bookmark

                        stream.writeEndDocument();
                    }
                    outFile.close();

                }

                static QRect crop(QString fullPathToLoadFrom,QString fullPathToSaveTo,QRgb colorForZBuffer)
                {
                    QDir dir;
                    bool isOK = dir.mkpath(fullPathToSaveTo);
                    if(!isOK)
                    {
                        QDir dd(fullPathToSaveTo);
                        if(!QFile::exists(dd.path()))
                            QMessageBox::warning(NULL,"Dir of this file can't be created",fullPathToSaveTo);
                    }

                    QDir dir2(fullPathToSaveTo);
                    if(dir2.exists())
                    {
                        bool isOK2 = dir2.rmdir(fullPathToSaveTo);
                        if(!isOK2)
                        {
                            QMessageBox::warning(NULL," couldnt remove",fullPathToSaveTo);
                        }
                    }
                    

                    // check the cache
                    QRect rect = getCachedRectIfValidOneExists(fullPathToLoadFrom);


                    static double g_skipped =0;
                    static double g_total = 1;
                    double percent =  g_skipped/g_total *100;

                    // reporting percentages
                    g_total++;
                    // we can skip if both cached rect is valid, and the output exists
                    if(/*!rect.isEmpty() &&*/ QFile::exists(fullPathToSaveTo))
                    {
                        g_skipped++;
                        qDebug() << "(" << percent << ")skipped " << fullPathToLoadFrom.toUtf8().data() << "\n";
                        return rect;//success!
                    }

                    qDebug() << "(" << percent << ")processed " << fullPathToLoadFrom.toUtf8().data() << "\n";

                    //load the image
                    QImage temp;
                    bool isLoaded = temp.load(fullPathToLoadFrom);

                    if(!isLoaded)
                        return QRect();// failure.

                    // getting the rect takes a long time - so skip it if we got a valid rect from the cache
                    if(rect.isEmpty())
                    {
                        rect = getBoundingNonBlackRectangle(temp);
                    }

                    QImage image;
                    if(rect.bottomRight().x() == -1)
                    {
                        image = QImage(1,1,QImage::Format_ARGB32_Premultiplied);
                        rect=QRect(1,1,0,0);
                    }
                    else
                    {
                        // generate images and assign
                        image = temp.copy(rect.x(),rect.y(),rect.width(),rect.height());
                    }

                    A2GASSERT(!image.isNull());
                    if(!image.isNull())
                    {
                        QImage image2 = image;

                        bool isOK1 = image2.save(fullPathToSaveTo,"png");

                        if(isOK1)
                        {
                            saveTimestampXml(fullPathToLoadFrom,rect);
                        }
                        else
                        {
                            QMessageBox::warning(NULL,"Failed to save",fullPathToSaveTo);
                        }
                    }

                    return rect;
                }

            }
        }
    }
}

