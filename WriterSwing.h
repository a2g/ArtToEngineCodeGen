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
#include "IWriter.h"
#include "writeAFile.h"
#include "writeInitialEnumFile.h"
#include "writeIdFile.h"
#include "writeSwingLoader.h"
#include "writeSwingBundle.h" 
#include "writeSwingClass.h"
#include "Dom2Location.h"
namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class WriterSwing : public IWriter
                {
                    QString writeAFile(const Dom2Location& location) const override
                    {
                        return generator::writeAFile(location);
                    }
                    QString writeInitialEnumFile(const Dom2Loader& loader) const override
                    {
                        return generator::writeInitialEnumFile(loader);
                    }
                    QString writeIFile(const Dom2Loader& camera) const override
                    {
                        return generator::writeIdFile('I',camera);
                    }
                    QString writeOFile(const Dom2Loader& camera) const override
                    {
                        return generator::writeIdFile('O',camera);
                    }
                    
                    QString writeLoader(const Dom2Loader& loader) const override
                    {
                        return generator::writeSwingLoader(loader);
                    }
                    QString writeBundle(const Dom2Bundle& camera) const override
                    {
                        return generator::writeSwingBundle(camera);
                    }
                    QString writeLoaderClass(const QString& package,const QString& className, int numberOfImages, QPoint res, int type) const override
                    {
                        return generator::writeSwingClass(package,className, numberOfImages, res, type);
                    }
                };
            }
        }
    }
}

