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
#include "StartingOddEnum.h"

namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class IdGenerator
                {
                protected:
                    QMap<QString, int> mapOfObjectNames;
                    int currentSeed;
                public:
                    IdGenerator(StartingOddEnum seed)
                            :currentSeed(seed)
                    {
                    }

                    int getIdForName(const QString& objectNamePassedIn)
                    {
                        QString  objectName = objectNamePassedIn.toUpper();
                        if(objectName.startsWith("X_", Qt::CaseInsensitive))
                            return -2;
                        if(mapOfObjectNames.find(objectName)!=mapOfObjectNames.end())
                            return mapOfObjectNames[objectName];
                        //else we've never seen it before
                        mapOfObjectNames[objectName] = currentSeed;
                        return (currentSeed++*2)+1;// yes, return old seed, then increment it
                    }
                };
            }
        }
    }
}






