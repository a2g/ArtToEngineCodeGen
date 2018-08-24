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
#include "writeClass.h"
#include "getPackage.h"
#include "allcaps\INITIALS.h"
#include "allcaps\NAMESPACE_BEGIN.h"
#include "allcaps\NAMESPACE_END.h"
NAMESPACE_BEGIN


static QString writeLoader(const Dom2Loader& l,bool isGwt)
{
    QString fullLocationPackage = l.parent().fullLocationPackage;
    QString loaderSeg = l.loaderSeg.toLower();
    QPoint rez = l.getResolutionOfFirstImageFound();
    QString s;
    s += QString("package %1.%2;\n").arg(fullLocationPackage).arg(loaderSeg);
    s += QString("\n"); 
    s += QString("import com.github.a2g.core.primitive.PointI;\n");
    s += QString("import com.github.a2g.core.interfaces.game.handlers.IOnEnqueueResources;\n");
    s += QString("import com.github.a2g.core.interfaces.nongame.platform.IBundleLoader;\n");
    s += QString("import com.github.a2g.core.interfaces.nongame.platform.IPlatformResourceBundle;\n");
    s += QString("import com.github.a2g.core.interfaces.nongame.presenter.IMasterPresenterFromBundle;\n");
    s += QString("import com.google.gwt.event.dom.client.LoadHandler;\n");
    s += QString("import %1.%2.InitialEnum;\n").arg(fullLocationPackage).arg(loaderSeg);
    s += QString("import javax.swing.Timer;\n");
    s += QString("import java.awt.event.ActionEvent;\n");
    s += QString("import java.awt.event.ActionListener;\n");
    
    s += QString("\n");
    s += QString("public class Loader implements IBundleLoader\n");
    s += QString("{\n");
    s += QString("  @Override\n");
    s += QString("  public int getImageWidth() { return %1;}\n").arg(rez.x());
    s += QString("  @Override\n");
    s += QString("  public int getImageHeight() { return %1; }\n").arg(rez.y());
    s += QString("  @Override\n");
    s += QString("  public int getLoaderEnum() { return %1; }\n").arg(l.type);
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
    s += QString("  public IPlatformResourceBundle getSingleBundle(int n)\n");
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
            s += QString("public static InitialEnum INITIALS(IOnEnqueueResources api){api.enqueueSingleBundle( new Loader().new INITIALS()); return new InitialEnum();}\n").arg(b.bundleName);
        else
            s += QString("public static String %1(IOnEnqueueResources api){api.enqueueSingleBundle( new Loader().new %1()); return \"%1\";}\n").arg(b.bundleName);
        s += writeClass(isGwt, getPackage(l),b.bundleName,b.parent().loaderSeg, b.getFrameCount(), QPoint(rez.x(), rez.y()), l.type);
    }
    s += QString("}\n");// deliberately leave off closing the class, since we need to add loader classes
    return s;
}
NAMESPACE_END