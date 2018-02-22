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
#include <QList>
#include <DomAnimation.h>
namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class DomCamera;
                class DomObject
                {
                public:
                    DomObject(DomCamera& parent) :theParent(&parent){}
                    
                    DomAnimation& addNewAnimation(const QString& animSeg = "" )
                    {
                        DomAnimation animation(*this);
                        list.append(animation);
                        list.last().animSeg = animSeg;
                        return list.last();
                    }

                    

                    QString objectSeg;
                    int prefix;

                    int getAnimationCount() const { return list.size(); }
                    const DomAnimation& getAnimationAt(int i) const { return list.at(i); }
                    const DomCamera& parent() const { return *theParent; };
                   // void setParent(DomCamera& parent) { theParent = &parent; }
                private:
                    QList<DomAnimation> list;
                    DomCamera* theParent;
                };
            }
        }
    }
}

