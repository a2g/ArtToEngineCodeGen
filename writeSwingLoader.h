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

#include <QVector>
#include "Dom2Location.h"
#include "writeLoadBundleElseIf.h"
#include "writeSwingClass.h"
#include "getPackage.h"
#include "INITIALS.h"
#include <NAMESPACE_BEGIN.h>
#include <NAMESPACE_END.h>
NAMESPACE_BEGIN


static QString writeSwingLoader(const Dom2Loader& l)
{
    QString fullLocationPackage = l.parent().fullLocationPackage;
    QString loaderSeg = l.loaderSeg.toLower();

    QString s;
    s += QString("package %1.%2;\n").arg(fullLocationPackage).arg(loaderSeg);

    s += QString("\n");
    s += QString("import com.github.a2g.core.interfaces.IOnQueueResourcesImpl;\n");
    s += QString("import com.github.a2g.core.interfaces.internal.IBundleLoader;\n");
    s += QString("import com.github.a2g.core.interfaces.internal.ISingleBundle;\n");
    s += QString("import com.github.a2g.core.interfaces.internal.IMasterPresenterFromBundle;\n");
    s += QString("import %1.%2.InitialEnum;\n").arg(fullLocationPackage).arg(loaderSeg);
    s += QString("import javax.swing.Timer;\n");
    s += QString("import java.awt.event.ActionEvent;\n");
    s += QString("import java.awt.event.ActionListener;\n");
    s += QString("import com.google.gwt.event.dom.client.LoadHandler;\n");
    s += QString("\n");
    s += QString("public class Loader implements IBundleLoader\n");
    s += QString("{\n");
    s += QString("  @Override\n");
    s += QString("  public int getImageWidth() { return %1;}\n").arg(l.resolutionX);
    s += QString("  @Override\n");
    s += QString("  public int getImageHeight() { return %1; }\n").arg(l.resolutionY);
    s += QString("  @Override\n");
    s += QString("  public int getLoaderEnum() { return 0; }\n");
    s += QString("  @Override\n");
    s += QString("  public int getNumberOfImagesInBundle(int bundleNumber)\n");
    s += QString("  {\n");
    s += QString("    if(bundleNumber==-1){}\n");
    auto iter = l.getBundles();
    for(int j = 0; j < l.getBundleCount(); j++)
    {
        auto b = iter.at(j);
        s += QString("     else if(bundleNumber== %1){ return %2;}\n").arg(j).arg(b.getFrameCount());
    }
    s += QString("    return 0;\n");
    s += QString("  }\n");
    s += QString("  @Override\n");
    s += QString("  public ISingleBundle getSingleBundle(int n)\n");
    s += QString("  {\n");
    s += QString("    if (n == -2) {}\n");
    for(int j = 0; j < l.getBundleCount(); j++)
    {
        auto b = iter.at(j);
        s += writeLoadBundleElseIf(b.bundleName,j);
    }
    s += QString("    return null;\n");//end loadImageBundle
    s += QString("  }\n");//end method
    s += QString("  @Override\n");
    s += QString("  public int getNumberOfBundles() { return %1; }\n").arg(l.getBundleCount());


    for(int j = 0; j < l.getBundleCount(); j++)
    {
        auto b = iter.at(j);
        if(b.bundleName==INITIALS)
            s += QString("public static InitialEnum INITIALS(IOnQueueResourcesImpl api){api.queueSingleBundle( new Loader().new INITIALS()); return new InitialEnum();}\n").arg(b.bundleName);
        else
            s += QString("public static String %1(IOnQueueResourcesImpl api){api.queueSingleBundle( new Loader().new %1()); return \"%1\";}\n").arg(b.bundleName);
        s += writeSwingClass(getPackage(l),b.bundleName,b.getFrameCount(), QPoint(l.resolutionX, l.resolutionY), 0);
    }
    s += QString("}\n");// deliberately leave off closing the class, since we need to add loader classes
    return s;
}
NAMESPACE_END