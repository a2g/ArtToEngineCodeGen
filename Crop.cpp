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
#include <QMessageBox>
#include <QImage>
#include <QRect>
#include <QSet>
#include <QList>
#include <QtGui/qrgb.h>
#include <QDomDocument>
#include <QTextStream>
#include <QTextBrowser>
#include <QVariant>
#include "OFile.h"
#include "IFile.h"
#include "LoaderAndResFilePair.h"
#include "AFile.h"

#include <QSplitter>

const char* STR_IMAGE = "croppedImage";
const char* STR_ZBUFFER = "croppedZBuffer";

using namespace com::github::a2g::generator;


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

    int max = image.width() > image.height()? image.width() : image.height();
    int top = 0;
    int left = 0;
    int bottom = image.height()-1;
    int right = image.width()-1;
    bool isToKeepAdvancingTop = true;
    bool isToKeepAdvancingLeft = true;
    bool isToKeepAdvancingBottom = true;
    bool isToKeepAdvancingRight = true;
    // this turns off the optimization :return QRect(QPoint(left,top),QPoint(right,bottom));

    // This is finding the bounding box by keeping shrinking the square (top,left,bottom,right)
    // until it runs into some non zero pixels.
    // we alternate between vertical and horizontal, because overall, this results in
    // fewer pixels to scan ( every horizontal line scanned can be two pixels shorter than the previous)
    for(int i=0;i<max;i++)
    {
        for(int k=left;k<=right && (isToKeepAdvancingTop || isToKeepAdvancingBottom);k++)
        {
            int colorTop = image.pixel(k,top)&RGB_MASK;
            int colorBottom = image.pixel(k,bottom)&RGB_MASK;
            isToKeepAdvancingTop &= (colorTop==0);
            isToKeepAdvancingBottom &= (colorBottom==0);
        }

        top += isToKeepAdvancingTop;
        bottom -= isToKeepAdvancingBottom;

        for(int k=top;k<=bottom && (isToKeepAdvancingLeft || isToKeepAdvancingRight);k++)
        {
            int colorLeft = image.pixel(left,k)&RGB_MASK;
            int colorRight = image.pixel(right,k)&RGB_MASK;
            isToKeepAdvancingRight &= (colorRight==0);
            isToKeepAdvancingLeft &= (colorLeft==0);
        }

        left += isToKeepAdvancingLeft;
        right -= isToKeepAdvancingRight;
    }

    QPoint topLeft(left,top);
    QPoint bottomRight(right, bottom);

    QRect toReturn(topLeft, bottomRight);

    // put corners pixels back
    image.setPixel(lastCol,lastRow,bottomRightPixel);
    image.setPixel(lastCol,0,topRightPixel);
    image.setPixel(0,lastRow,bottomLeftPixel);
    image.setPixel(0,0,topLeftPixel);

    return toReturn;
}


QImage generateZBufferRepresentation(QImage image, QRgb zbufferColor)
{
    QImage zbuf = image.createHeuristicMask(false);
    zbuf.invertPixels();
    QImage alphed(zbuf.width(), zbuf.height(), QImage::Format_ARGB32_Premultiplied);
    alphed.fill(zbufferColor);// if aplha, then fill will overwrite alpha
    alphed.setAlphaChannel(zbuf);
    return alphed;
}

QRect crop(QString fullPathToLoadFrom, QString fullPathToSaveTo, QRgb colorForZBuffer)
{
    QDir dir;
    bool isOK = dir.mkpath(fullPathToSaveTo);
    if(!isOK)
    {
        QDir dd(fullPathToSaveTo);
        if(!QFile::exists(dd.path()))
            QMessageBox::warning(NULL, "Dir of this file can't be created", fullPathToSaveTo);
    }

    QDir dir2(fullPathToSaveTo);
    if(dir2.exists())
    {
        bool isOK2 = dir2.rmdir(fullPathToSaveTo);
        if(!isOK2)
        {
            QMessageBox::warning(NULL, " couldnt remove", fullPathToSaveTo);
        }
    }

    QRect rect;
    bool isCropping = true;
    if(isCropping)
    {
        QImage temp;
        bool isLoaded = temp.load(fullPathToLoadFrom);

        A2GASSERT(isLoaded);
        if(isLoaded)
        {
            QString croppedZBufferFilename = fullPathToLoadFrom.left(fullPathToLoadFrom.length() -4) + "_CroppedZBuffer.bmp";
            rect = getBoundingNonBlackRectangle(temp);

            QImage image;
            QImage zbuf;
            if(rect.bottomRight().x() == -1)
            {
                image = QImage(1,1,QImage::Format_ARGB32_Premultiplied);
                zbuf = QImage(1,1, QImage::Format_ARGB32_Premultiplied);
                rect=QRect(1,1,0,0);
            }
            else
            {
                // generate images and assign
                image = temp.copy(rect.x(), rect.y(), rect.width(), rect.height());
            }
            A2GASSERT(!image.isNull());
            if(!image.isNull())
            {
                QImage image2 = image;

                bool isOK1 = image2.save(fullPathToSaveTo,"png");
                (isOK1 = isOK1);
                 if(!isOK1)
                    QMessageBox::warning(NULL, "Failed to save",fullPathToSaveTo);
            }
        }
    }else
    {
        QFile::remove(fullPathToSaveTo);
        QFile::copy(fullPathToLoadFrom, fullPathToSaveTo);
    }

    return rect;
}
