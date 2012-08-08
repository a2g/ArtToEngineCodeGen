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

QPoint Crop(QString folder, QString loadFrom, QString saveTo, QRgb colorForZBuffer)
{
    QDir loadMe(loadFrom);


    QString loadPath = loadMe.canonicalPath();
    QString saveAsCroppedPath = folder + "/" + saveTo;

    int x = 0;
    int y = 0;
    QImage image;
    QImage zbuf;
    bool isLoaded = false;
    QFile xmlFile(loadPath+".xml");


    if(!isLoaded)
    {
        QImage temp;
        isLoaded = temp.load(loadPath);

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
            QString croppedZBufferFilename = loadPath.left(loadPath.length() -4) + "_CroppedZBuffer.bmp";
            QString croppedFilename = saveAsCroppedPath;
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

                bool isOK1 = image2.save(croppedFilename,"png");
                (isOK1 = isOK1);
                A2GASSERT(isOK1);
                zbuf = GenerateZBufferRepresentation(image, colorForZBuffer);
                QImage zbuf2 = zbuf;

                bool isOK2 = zbuf2.save(croppedZBufferFilename,"bmp");
                (isOK2 = isOK2);
                A2GASSERT(isOK2);

                image = image2;
                zbuf = zbuf2;
            }

            QDomDocument doc("MyML");
            QDomElement root = doc.createElement("MyML");
            doc.appendChild(root);
            root.setAttribute("x", QVariant(x).toString() );
            root.setAttribute("y", QVariant(y).toString() );
            root.setAttribute(STR_IMAGE, croppedZBufferFilename );
            root.setAttribute(STR_ZBUFFER, croppedZBufferFilename );

            if (xmlFile.open(QFile::WriteOnly | QFile::Truncate)) 
            {
                QTextStream out(&xmlFile);
                out << doc.toString() << endl;
            }
            xmlFile.close();
        }
    }

    return QPoint(x,y);
}
