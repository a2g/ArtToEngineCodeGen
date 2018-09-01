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
#include <Qvector>
#include "IdGenerator.h"
#include "getRealObjectSeg.h"
#include "Dom2Location.h"
#include "allcaps\NAMESPACE_BEGIN.h"
#include "allcaps\NAMESPACE_END.h"
NAMESPACE_BEGIN
static QString writeHotspotFile(const Dom2Loader& l)
{
    // write file.
    QString fullLocationPackage = l.parent().fullLocationPackage.toLower();

    QString package = QString("%1.%2").arg(fullLocationPackage).arg(l.loaderSeg.toLower());
    QString s;
    s += QString("package %1;\n").arg(package);
    s += QString("\n");
    s += QString("     public enum H\n");
    s += QString("     {\n");
    for(auto iter = l.getIds().begin(); iter!=l.getIds().end();iter++)
    {
        if(iter.key().endsWith("_"))
        {
            s += QString("        %1(%2),\n").arg(iter.key().toUpper()).arg(iter.value());
        }
    }
    s += ("        ;\n");
    s += ("        public short code;");
    s += ("        H(int code) {\n");
    s += ("         this.code=(short)code;\n");
    s += ("        }\n");
    s += ("    }\n");
  
    return s;
}
NAMESPACE_END