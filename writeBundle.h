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
#include <QString>
#include <QDir>
#include <QVector>
#include <vector>
#include "getObjectPlusAnim.h"
#include "getObjectPlusAnimPlusNumber.h"
#include "allcaps\NAMESPACE_BEGIN.h"
#include "allcaps\NAMESPACE_END.h"
#include <Dom2Location.h>
#include <IdGenerator.h>
NAMESPACE_BEGIN
static  QString  writeBundle(const Dom2Bundle& b)
{
    QString fullPackage = b.parent().parent().fullLocationPackage;
    QString loaderSeg = b.parent().loaderSeg;
    auto& loader = b.parent();
    
    QString s;
    s += QString("package %1.%2;\n").arg(fullPackage).arg(loaderSeg);
    s += QString("\n");
    s += QString("import com.github.a2g.core.interfaces.internal.IMasterPresenterFromBundle;\n");
    s += QString("import com.github.a2g.core.platforms.swing.dependencies.PackagedImageForSwing;\n");
    s += QString("import com.google.gwt.event.dom.client.LoadHandler;\n");
    s += QString("\n");
    s += QString("public class %1\n").arg(b.bundleName);
    s += QString("{\n");
    s += QString("    public static class MyRes{\n");
    s += QString("        public static final MyRes RESOURCE = new MyRes();");
    s += QString("        \n");
    for(int k = 0; k < b.getFrameCount(); k++)
    {
        const Dom2Frame& f = b.getFrameAt(k);
        auto objectPlusAnimToUpper = f.realObjectSegToUpper + "_" + f.animSegToUpper;
        auto objectPlusAnimToLower = objectPlusAnimToUpper.toLower();
        std::string debug = f.fullimagePath.toUtf8().data();

        s += QString("        public String %1_%2(){ return \"%3\"\n").arg(objectPlusAnimToLower).arg(k).arg(f.fullimagePath);
        s += QString("        ;}\n");
    }

    s += QString("    }\n");
    s += QString("    public static boolean addImage(IMasterPresenterFromBundle api, LoadHandler lh, int i)\n");
    s += QString("    {\n");
    s += QString("        final MyRes res = MyRes.RESOURCE;\n");
    s += QString("        if (i == -1) {}\n");

    for(int i = 0; i<b.getFrameCount(); i++)
    {
        const Dom2Frame& f = b.getFrameAt(i);

        auto realObjectSegToUpper = f.realObjectSegToUpper;
        auto objectPlusAnimToUpper = realObjectSegToUpper + "_" + f.animSegToUpper;
        auto objectPlusAnimToLower = objectPlusAnimToUpper.toLower();
        auto ocode = loader.getIds()[realObjectSegToUpper];

        s += QString("else if (i == %1)").arg(i);
        s += QString(" return api.addImageForEitherInventoryOrScene(lh, %1,%2,%3,%4,%5, \"%6\",\"%7\",(short)%8,\"%9\",new PackagedImageForSwing(res.%10_%11()));\n").arg(f.objectPrefix).arg(f.x).arg(f.y).arg(f.w).arg(f.h).arg(realObjectSegToUpper).arg(f.animSegToUpper).arg(ocode).arg(objectPlusAnimToUpper).arg(objectPlusAnimToLower).arg(i);
    }

    s += QString("        return false;"                                                              "\n");
    s += QString("    }"                                                                               "\n");
    s += QString("}"                                                                                   "\n");

    return s;
}
NAMESPACE_END