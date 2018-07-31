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
#include <QVector>
#include <FileSystem.h>
#include <Dom2Location.h>
#include <debugapi.h>
#include "IInscriber.h" 
#include "getObjectPlusAnim.h"
 
#include "getPackage.h"
#include "convertBundleToGwt.h"

#include "allcaps\LOADERFORINVENTORY.h"
#include "allcaps\INITIALS.h"
#include "allcaps\NAMESPACE_BEGIN.h"
#include "allcaps\NAMESPACE_END.h"
#include "allcaps\AFILE.h"

NAMESPACE_BEGIN
 

static QPair<std::string,std::string> getPair(QString a, QString b) {
    return QPair<std::string,std::string>(a.toUtf8().data(), b.toUtf8().data());
}

static void output(QMap<QString, Dom2Bundle>::Iterator& iter)
{
    auto& key = iter.key();
    auto& b = iter.value();
    OutputDebugStringA("\t");
    OutputDebugStringA(b.bundleName.toUtf8());
    OutputDebugStringA(" ");
    OutputDebugStringA(key.toUtf8());
    OutputDebugStringA(" ");
    char buf[100]={};
    OutputDebugStringA(_itoa(b.getFrameCount(),buf,10));

    if(b.bundleName==INITIALS && b.getFrameCount() == 1)
    {
        OutputDebugStringA(" ");
        OutputDebugStringA(b.getFrameAt(0).realObjectSegToUpper.toUtf8());
        OutputDebugStringA("_");
        OutputDebugStringA(b.getFrameAt(0).animSegToUpper.toUtf8());
    }
    OutputDebugStringA("\n");
}

static QVector<QPair<std::string,std::string>> buildFilenamesAndContents(const Dom2Location& location, const IInscriber& w)
{
    QVector<QPair<std::string,std::string>> builtFiles;
    auto locPath = location.locationPath.toLower();

    {
        // A.java
        auto afile = w.writeAFile(location);
        builtFiles.push_back(getPair(afile,locPath+"/"+AFILE.toLower()+"/A.java"));
    }

  

    for(int i = 0; i < location.getLoaderCount(); i++)
    {
        auto loader = location.getLoaderAt(i);
        auto loaderSeg = loader.loaderSeg;
        OutputDebugStringA(loaderSeg.toUtf8().data());
        OutputDebugStringA("\n");

        {
            // InitialEnum.java
            auto afile = w.writeInitialEnumFile(loader);
            builtFiles.push_back(getPair(afile,locPath + "/" + loaderSeg +"/InitialEnum.java"));
        }

        if(loader.getBundleCount()>0){
            // write the loader file
            auto loaderFile = w.writeLoader(loader);
            builtFiles.push_back(getPair(loaderFile,locPath + "/" + loaderSeg + "/Loader.java"));
        }


        // ..and all its bundles
        auto map = loader.map;
        for(auto iter = map.begin();iter!=map.end();iter++)
        {
            auto b = iter.value();
            output(iter);

            // write each bundle file
            if(b.getFrameCount()>0)// ignore zero bundles (they generate to many warnings with unused imports)
            {
                auto bundleFile = w.writeBundle(b);
                builtFiles.push_back(getPair(bundleFile,locPath + "/" + loaderSeg + "/" + b.bundleName + ".java"));
            }
        }

        // and maye an I or an O file
        if(loader.type == LoaderTypeEnum::INVENTORY)
        {
            // I.java
            auto ifile = w.writeIFile(loader);
            builtFiles.push_back(getPair(ifile,locPath + "/" + loaderSeg + "/I.java"));
        }
        else
        {
            if(loader.mapOfIds.size()>0)
            {
                // O.java
                auto ofile = w.writeOFile(loader);
                builtFiles.push_back(getPair(ofile,locPath+"/"+loaderSeg+"/O.java"));
            }
        }
    }

    return builtFiles;
}

NAMESPACE_END
