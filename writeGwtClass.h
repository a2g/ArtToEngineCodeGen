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
#include "DomAnimation.h"
#include <NAMESPACE_BEGIN.h>
#include <NAMESPACE_END.h>
NAMESPACE_BEGIN

static QString writeGwtClass(const QString& package,const QString& loaderName)
{
    QString f;
    f += QString("public static ISingleBundle %1(){return new Loader().new %1();}\n").arg(loaderName);
    f += QString("public class %1 implements ISingleBundle\n").arg(loaderName);
    f += QString("{\n");
    f += QString("  public %1() {}\n").arg(loaderName);
    f += QString("  @Override public int getBundleNumber() { return 0; }\n");
    f += QString("  @Override public String toString() { return \"%1\"; }\n").arg(loaderName);
    f += QString("  @Override public int getSize() { return 1; }\n");
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
    f += QString("                %1.%2.addImage(api, lh, counter);\n").arg(package).arg(loaderName);
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

    return f;
}
NAMESPACE_END