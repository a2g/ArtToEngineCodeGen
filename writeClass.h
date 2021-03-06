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
#include "allcaps\NAMESPACE_BEGIN.h"
#include "allcaps\NAMESPACE_END.h"
#include "allcaps\LOADERFORINVENTORY.h"

NAMESPACE_BEGIN
static QString writeClass(bool isGwt, const QString& package,const QString& bundleName,const QString& loaderSeg, int numberOfImages, QPoint resolution, int type)
{
    bool isInv = bundleName.toUpper().contains("INV");
    QString f;
    if(isGwt)
    {
        f += QString("public class %1 implements IPlatformResourceBundle\n").arg(bundleName);
        f += QString("{\n");
        f += QString("  public %1() {}\n").arg(bundleName);
        f += QString("  @Override public int getTypeOfLoader() { return %1; }\n").arg(type);
        f += QString("  @Override public String toString() { return \"%1\"; }\n").arg(bundleName + "_" + loaderSeg);
        f += QString("  @Override public int getSize() { return %1; }\n").arg(numberOfImages);
        f += QString("  @Override public PointI getImageResolution(){ return new PointI(%1,%2);}\n").arg(resolution.x()).arg(resolution.y());
        f += QString("  @Override public int loadImageBundle(final LoadHandler lh, final IMasterPresenterFromBundle api)\n");
        f += QString("  {\n");
        f += QString("    GWT.runAsync\n");
        f += QString("    (\n");
        f += QString("      new RunAsyncCallback()\n");
        f += QString("      {\n");
        f += QString("        public void onSuccess()\n");
        f += QString("        {\n");
        f += QString("          // Schedule the IncrementalCommand instance to run when\n");
        f += QString("          // control returns to the event loop by returning 'true'\n");
        f += QString("          Scheduler.get().scheduleFixedDelay\n");
        f += QString("          (\n");
        f += QString("            new Scheduler.RepeatingCommand()\n");
        f += QString("            {\n");
        f += QString("              int counter=0;\n");
        f += QString("              @Override\n");
        f += QString("              public boolean execute()\n");
        f += QString("              {\n");
        f += QString("                %1.%2.addImage(api, lh, counter);\n").arg(package).arg(bundleName);
        f += QString("                return (++counter<getSize());\n");
        f += QString("              }\n");
        f += QString("            }\n");
        f += QString("            ,0\n");
        f += QString("          );\n");
        f += QString("        }\n");
        f += QString("        public void onFailure(Throwable caught)\n");
        f += QString("        {\n");
        f += QString("		   lh.onLoad(null);\n");
        f += QString("        }\n");
        f += QString("      }\n");
        f += QString("    );\n");// end GWT.runAsync
        f += QString("    return 0;\n");
        f += QString("  }\n"); // end method (loadImageBundle)
        f += QString("}\n"); // end class

    }
    else
    {
        f += QString("public class %1 implements IPlatformResourceBundle\n").arg(bundleName);
        f += QString("{\n");
        f += QString("  Timer timer;\n");
        f += QString("  public %1() {}\n").arg(bundleName);
        f += QString("  @Override public int getTypeOfLoader() { return %1; }\n").arg(type);
        f += QString("  @Override public String toString() { return \"%1\"; }\n").arg(bundleName + "_" + loaderSeg);
        f += QString("  @Override public int getSize() { return %1; }\n").arg(numberOfImages);
        f += QString("  @Override public PointI getImageResolution(){ return new PointI(%1,%2);}\n").arg(resolution.x()).arg(resolution.y());
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
    }
    return f;

}
NAMESPACE_END