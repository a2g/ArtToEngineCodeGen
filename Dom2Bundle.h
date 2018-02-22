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
#include <Qstring>
#include <QFile>
#include <QVector>
#include "Dom2Frame.h"

namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class Dom2Loader;

                class Dom2Bundle
                {
                public:
                    Dom2Bundle(){};
                    Dom2Bundle(Dom2Loader& parent):theParent(&parent) {};

                    QString bundleName;

                    void addFrame(Dom2Frame& frame)
                    {
                        list.push_back(frame);
                    }

                    Dom2Bundle& addFrame(int prefix,int x,int y,int w,int h,QString realObjectSegtoUpper,QString animSegtoUpper, QString fullimagePath)
                    {
                        list.push_back(Dom2Frame(prefix, x, y, w, h, realObjectSegtoUpper, animSegtoUpper, fullimagePath));
                        return *this;
                    }

                    const Dom2Loader& parent() const { return *theParent; }
                    const Dom2Frame& getFrameAt(int i) const { return list.at(i); }
                    QVector<Dom2Frame>& getFrames() { return list; }
                    int getFrameCount() const { return list.size(); }
                private:
                    QVector<Dom2Frame> list; // ensures this is a vector so we get frames in order.
                    Dom2Loader* theParent;
                };
            }
        }
    }
}

