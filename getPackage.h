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
#include <time.h>
#include <locale>
#include <QString>
#include "DomLocation.h"
#include "Dom2Loader.h"
#include "GetObjectPlusAnim.h"

namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                static QString getPackage(const DomCamera& c)
                {
                    auto l = c.parent();
                    QString cameraPackage = QString("%1.%2").arg(l.fullLocationPackage).arg(c.cameraSeg);
                    return cameraPackage;
                }

                static QString getPackage(const Dom2Loader& loader)
                {
                    auto l = loader.parent();
                    QString loaderPackage = QString("%1.%2").arg(l.fullLocationPackage).arg(loader.loaderSeg);
                    return loaderPackage;
                }
            }
        }
    }
}
