/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
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


QRect GetBoundingNonBlackRectangle(QImage image)
{
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

    return toReturn;
}


QImage GenerateZBufferRepresentation(QImage image, QRgb zbufferColor)
{
    QImage zbuf = image.createHeuristicMask(false);
    zbuf.invertPixels();
    QImage alphed(zbuf.width(), zbuf.height(), QImage::Format_ARGB32_Premultiplied);
    alphed.fill(zbufferColor);// if aplha, then fill will overwrite alpha
    alphed.setAlphaChannel(zbuf);
    return alphed;
}

QPoint Crop(QString fullPathToLoadFrom, QString fullPathToSaveTo, QRgb colorForZBuffer)
{
    QDir dir;
    bool isOK = dir.mkpath(fullPathToSaveTo);
    if(!isOK)
    {
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
    int x = 0;
    int y = 0;
    QImage image;
    QImage zbuf;
    bool isLoaded = false;
    QFile xmlFile(fullPathToSaveTo+".xml");


    if(!isLoaded)
    {
        QImage temp;
        isLoaded = temp.load(fullPathToLoadFrom);

        // we need to set black in all the four corners because of createHeiristicMask
        // which is used in GenerateZBufferRepresentation, this takes a color key from the four corners.
        int lastCol = temp.width()-1;
        int lastRow = temp.height()-1;
        temp.setPixel(lastCol,lastRow,0);
        temp.setPixel(lastCol,0,0);
        temp.setPixel(0,lastRow,0);
        temp.setPixel(0,0,0);
        A2GASSERT(isLoaded);
        if(isLoaded)
        {
            QString croppedZBufferFilename = fullPathToLoadFrom.left(fullPathToLoadFrom.length() -4) + "_CroppedZBuffer.bmp";
            QRect rect = GetBoundingNonBlackRectangle(temp);

            if(rect.bottomRight().x() == -1)
            {
                image = QImage(1,1,QImage::Format_ARGB32_Premultiplied);
                zbuf = QImage(1,1, QImage::Format_ARGB32_Premultiplied);
                x = 1;
                y = 1;
            }
            else
            {
                x = rect.x();
                y = rect.y();

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
    }

    return QPoint(x,y);
}
