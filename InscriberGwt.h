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
#include "IInscriber.h"

#include "writeInitialEnumFile.h"
#include "writeIFileOrOFile.h"
#include "convertLoaderToGwt.h"
#include "convertBundleToGwt.h" 
#include "getObjectPlusAnim.h"
#include "writeLoader.h"
#include "writeBundle.h" 
#include "writeClass.h"
#include "Dom2Location.h"
#include "writeICombinationsFile.h"
#include "writeHotspotFile.h"
namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class InscriberGwt: public IInscriber
                {
                    static const bool isGwt = true;

                    QString writeInitialEnumFile(const Dom2Loader& loader) const override
                    {
                        return generator::writeInitialEnumFile(loader);
                    }

                    QString writeIFile(const Dom2Loader& camera) const override
                    {
                        return generator::writeIFileOrOFile('I',camera);
                    }
                    QString writeOFile(const Dom2Loader& camera) const override
                    {
                        return generator::writeIFileOrOFile('O',camera);
                    }

                    QString writeLoader(const Dom2Loader& loader) const override
                    {
                        auto s = generator::writeLoader(loader, isGwt);
                        return generator::convertLoaderToGwt(s);
                    }
                    QString writeBundle(const Dom2Bundle& camera) const override
                    {
                        auto s = generator::writeBundle(camera);
                        return generator::convertBundleToGwt(s);
                    }
                    QString writeClass(const QString& package,const QString& className,const QString& loaderSeg, int numberOfImages,QPoint res,int type) const override
                    {
                        return generator::writeClass(isGwt, package,className,loaderSeg, numberOfImages,res,type);
                    }
                    QString writeICombinationsFile(const Dom2Loader& camera) const override
                    {
                        return generator::writeICombinationsFile(camera);
                    }
                    QString writeHotspotFile(const Dom2Loader& camera) const override
                    {
                        return generator::writeHotspotFile(camera);
                    }
                };
            }
        }
    }
}

