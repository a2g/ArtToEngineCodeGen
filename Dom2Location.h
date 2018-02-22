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
#include <QString>
#include <QFile>
#include <QList>
#include <QSet>
#include "Dom2Loader.h"
#include <set>
namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class Dom2Location
                {    
                    
                public:
                    Dom2Location()
                    {
                    }

                    Dom2Location(const QString& fullLocaitonPackage, const QString& locationPath)
                        :fullLocationPackage(fullLocaitonPackage)
                        , locationPath(locationPath)
                        
                    {
                    }


                    struct classcomp {
                        bool operator() (const QString& lhs,const QString& rhs) const
                        {
                            return lhs>rhs;
                        }
                    };
                    std::set<QString, classcomp> set;
                    QString fullLocationPackage;
                    QString locationPath;

                    Dom2Loader& addNewLoader(const QString loaderSeg)
                    {
                        Dom2Loader loader(*this);
                        list.append(loader);
                        list.last().loaderSeg = loaderSeg;
                        return list.last();
                    }
                    void clear()
                    {
                        list.clear();
                         
                    }
                    const Dom2Loader& getLoaderAt(int i) const { return list.at(i); }
                    const int getLoaderCount() const { return list.size(); }
                    Dom2Location& asRef() { return *this; }
                    void addAnimCode(QString objectPlusAnim){ set.insert(objectPlusAnim); }
                private:
                    QList<Dom2Loader> list;
                   
                };
            }
        }
    }
}

