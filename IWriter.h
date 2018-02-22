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
#include "DomLocation.h"
#include "Dom2Loader.h"
namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                struct IWriter
                {
                    virtual QString writeAFile(const Dom2Location& location) const = 0;
                    virtual QString writeIFile(const Dom2Loader& camera) const = 0;
                    virtual QString writeInitialEnumFile(const Dom2Loader& loader) const = 0;
                    virtual QString writeOFile(const Dom2Loader& camera) const = 0;
                    virtual QString writeLoader(const Dom2Loader& loader) const = 0;
                    virtual QString writeBundle(const Dom2Bundle& bundle) const = 0;
                    virtual QString writeLoaderClass(const QString& package,const QString& className,int numberOfImages,QPoint res,int type) const =0;
                };
            }
        }
    }
}






