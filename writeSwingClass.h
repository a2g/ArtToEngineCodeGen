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
#include <QPoint>
#include <NAMESPACE_BEGIN.h>
#include <NAMESPACE_END.h>
#include "LOADERFORINVENTORY.h"
NAMESPACE_BEGIN
static QString writeSwingClass(const QString& package,const QString& bundleName, int numberOfImages, QPoint resolution, int type)
{
    bool isInv = bundleName.toUpper().contains("INV");
    QString f;
    f += QString("public class %1 implements ISingleBundle\n").arg(bundleName);
    f += QString("{\n");
    f += QString("  Timer timer;\n");
    f += QString("  public %1() {}\n").arg(bundleName);
    f += QString("  @Override public int getImageWidth() { return %1; }\n").arg(resolution.x());
    f += QString("  @Override public int getImageHeight() { return %1; }\n").arg(resolution.y());
    f += QString("  @Override public int getLoaderEnum() { return %1; }\n").arg(type);
    f += QString("  @Override public String toString() { return \"%1\"; }\n").arg(bundleName);
    f += QString("  @Override public int getSize() { return %1; }\n").arg(numberOfImages);
    f += QString("  @Override public int loadImageBundle(final LoadHandler lh, final IMasterPresenterFromBundle api)\n");
    f += QString("  {\n");
    f += QString("    timer = new Timer\n");
    f += QString("    (\n");
    f += QString("      0\n");
    f += QString("      , new ActionListener()\n");
    f += QString("      {\n");
    f += QString("        int counter = 0;\n");
    f += QString("        @Override\n");
    f += QString("        public void actionPerformed(ActionEvent e)\n");
    f += QString("        {\n");
    f += QString("          %1.%2.addImage(api, lh, counter + 0);\n").arg(package).arg(bundleName);
    f += QString("          counter++;\n");
    f += QString("          if (counter == getSize())\n");
    f += QString("            timer.stop();\n");
    f += QString("          lh.onLoad(null);\n");
    f += QString("        }\n");
    f += QString("      }\n");
    f += QString("    );\n");
    f += QString("    timer.setInitialDelay(0);\n");
    f += QString("    timer.start();\n");
    f += QString("    return 0;\n");
    f += QString("  }\n");
    f += QString("}\n");

    return f;

}
NAMESPACE_END