#pragma once
#include <QPoint>
namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                static QPoint getWidthAndHeightFromImageName(QString imageName)
                {
                    int lastSlash = imageName.lastIndexOf("/");
                    if(lastSlash!=-1)
                    {
                        imageName = imageName.mid(lastSlash+1);//just beyond last slash - at worst this is null pointer so still safe
                    }
                    imageName = imageName.replace("_"," "); // this allows toInt to work, since it splits on whitespace
                    int i = imageName.indexOf("x");
                    if(i!=-1)
                    {
                        QString first = imageName.mid(1,i-1);
                        //                            ^
                        //                         this is the underscore preceding width, gwt needs underscore
                        QString last = imageName.mid(i+1);

                        // atoi is better than QString::toInt (see here http://en.cppreference.com/w/cpp/string/byte/atoi)
                        int width = atoi(first.toAscii());
                        int height = atoi(last.toAscii());
                        return QPoint(width,height);
                    }
                    return QPoint(320,180);// the default
                }
            }
        }
    }
}
