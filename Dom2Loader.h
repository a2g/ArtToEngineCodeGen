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
#include <QMap>
#include <assert.h>
#include "Dom2Bundle.h"
#include "allcaps\INITIALS.h"
#include "getWidthAndHeightFromImageName.h"
namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                enum LoaderTypeEnum
                {
                    SHARED = 0,
                    NORMAL =1,
                    INVENTORY = 2
                    
                };
                struct IInscriber;
                class Dom2Location;
                class Dom2Loader
                {
                public:
                    Dom2Loader(Dom2Location& parent)
                        :theParent(&parent)
                        ,type(LoaderTypeEnum::SHARED)
                        {
                        
                    }

                    
                    QString loaderSeg;
                    LoaderTypeEnum type;
                    
                    Dom2Bundle& addNewBundle(const char* name)
                    {
                        QString bundleName = QString(name).toUpper();
                        if(map.contains(bundleName))
                        {
                            return map[bundleName];
                        }
                        Dom2Bundle b(*this);
                        map[bundleName] = b;
                        map[bundleName].bundleName = bundleName;
                        return map[bundleName];
                    }
                    const Dom2Location& parent() const { return *theParent; }
                    QList<Dom2Bundle> getBundles() const { return map.values(); } 
                    const Dom2Bundle& getConstInitialBundle() const { return map[INITIALS]; }
                    int getBundleCount() const{ return map.size(); }
                    void addNewId(QString name,int id){ mapOfIds[name.toUpper()] = id; }
                    const QMap<QString,int>& getIds() const { return mapOfIds; } 
                    void removeBundle(const QString& bundleName)
                    {
                        map.remove(bundleName);
                    }
                    QPoint getResolutionOfFirstImageFound() const
                    {
                        QPoint point(320,180);
                        if(map.size()>0)
                        {
                            auto& bundle = map.begin().value();
                            if(bundle.getFrameCount()>0)
                            {
                                auto& frame = bundle.getFrameAt(0);
                                point = getWidthAndHeightFromImageName(frame.fullimagePath);
                            }
                        }
                        return point;
                    }
                private:
                    QMap<QString,int> mapOfIds;
                    QMap<QString, Dom2Bundle> map;//private to ensure parents are set on Dom2Bundle
                    Dom2Location* theParent;
                    friend QVector<QPair<std::string,std::string>> buildFilenamesAndContents(const Dom2Location& location,const IInscriber& w);
                };
            }
        }
    }
}

