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
#include "DomObject.h"
namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class DomLocation;
                class DomCamera
                {
                public:
                    DomCamera(DomLocation& parent) :theParent(&parent){};

                    QString cameraSeg;
                    QString cameraPath;

                    DomObject& addNewObject(const QString& objectSeg = "")
                    {
                        DomObject object(*this);
                        list.append(object);
                        list.last().objectSeg = objectSeg;
                        return list.last();
                    }
                    const DomLocation& parent() const { return *theParent; }
                    //void setParent(DomLocation& parent) { theParent = &parent; }
                    const DomObject& getObjectAt(int i) const { return list.at(i); }
                    int getObjectCount() const{ return list.size(); }
                    
                private:
                    QList<DomObject> list;//private to ensure parents are set on DomObject
                    DomLocation* theParent;
                };
            }
        }
    }
}

