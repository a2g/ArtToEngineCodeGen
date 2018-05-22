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
#include "Dom2Location.h"
#include "allcaps\AFile.h"
#include <QSet.h>
#include "getObjectPlusAnim.h"
#include "allcaps\NAMESPACE_BEGIN.h"
#include "allcaps\NAMESPACE_END.h"
NAMESPACE_BEGIN
static QString writeInitialEnumFile(const Dom2Loader& l)
{
    QString fullLocationPackage = l.parent().fullLocationPackage;
    QString s;
    s += QString("package %1.%2;\n").arg(fullLocationPackage).arg(l.loaderSeg);
    s += QString("public class InitialEnum\n");
    s += ("{\n");

    QSet<QString> animations;
    {
        auto& bundles = l.getBundles();
        for(int i=0;i<bundles.size();i++)
        {
            for(int j = 0;j<bundles[i].getFrameCount();j++)
            {
                auto frame = bundles[i].getFrameAt(j);
                QString animationName= frame.realObjectSegToUpper+"_"+frame.animSegToUpper;
                animations.insert(animationName);
            }
        }
    }

    for(auto iter = animations.begin();iter!=animations.end();iter++)
    {
        s += QString("  public final String %1 =\"%1\";\n").arg(*iter);
    }
    s += ("}\n");

    return s;
}

NAMESPACE_END






