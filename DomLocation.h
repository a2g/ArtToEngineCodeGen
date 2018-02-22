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
#include "DomCamera.h"

namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class DomLocation
                {    
                    
                public:
                    DomLocation()
                    {
                    }

                    DomLocation(const QString& package, const QString& locationSeg)
                        :fullLocationPackage(package)
                        ,locationSeg(locationSeg)
                    {
                    }

                    QString locationSeg;
                    QString locationPath;
                    QString fullLocationPackage;

                    DomCamera& addNewCamera(const QString& cameraSeg)
                    {
                        DomCamera camera(*this);
                        list.append(camera);
                        list.last().cameraSeg = cameraSeg;
                        return list.last();
                    }
                    void clear()
                    {
                        list.clear();
                        locationSeg = locationPath = fullLocationPackage = "";
                    }
                    const DomCamera& getCameraAt(int i) const { return list.at(i); }
                    const int getCameraCount() const { return list.size(); }
                    DomLocation& asRef() { return *this; }
                private:
                    QList<DomCamera> list;
                };
            }
        }
    }
}

