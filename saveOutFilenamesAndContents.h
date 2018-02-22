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
#include <QTextStream>
#include <QMap>
#include <QString>
#include <QDir>
#include <QVector>
#include "DomAnimation.h"
#include <vector>

namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                static void saveOutFilenamesAndContents(QVector<QPair<std::string,std::string>> list)
                {
                    for(int i=0;i<list.size();i++)
                    {
                        auto code = list[i].first;
                        auto filename = list[i].second;
                        
                        QFile file(filename.c_str());
                        QFileInfo info(file);
                        QDir dir;

                        auto folder = info.dir().absolutePath().toUtf8();
                        dir.mkpath(folder);
                        
                        if(!file.open(QFile::WriteOnly | QFile::Truncate))
                            throw new std::bad_exception;

                        QTextStream f(&file);
                        f << list[i].first.c_str();
                        file.close();
                    }
                }
            }
        }
    }
}
