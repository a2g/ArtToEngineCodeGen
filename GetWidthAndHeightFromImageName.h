/*
 * Copyright 2018 Anthony Cassidy
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
#include <QPoint>
#include <QString>
#include "allcaps\NAMESPACE_BEGIN.h"
#include "allcaps\NAMESPACE_END.h"
NAMESPACE_BEGIN
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
        QString first = imageName.mid(9,i-1);
        //                            ^
        //                         this is the underscore preceding width, gwt needs underscore
        QString last = imageName.mid(i+1);

        // atoi is better than QString::toInt (see here http://en.cppreference.com/w/cpp/string/byte/atoi)
        int width = atoi(first.toUtf8());
        int height = atoi(last.toUtf8());
        return QPoint(width,height);
    }
    return QPoint(320,180);// the default
}
NAMESPACE_END