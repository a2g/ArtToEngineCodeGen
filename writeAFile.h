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
static QString writeAFile(const Dom2Location& l)
{


    QString fullLocationPackage = l.fullLocationPackage;
    QString s;
    s += QString("package %1.%2;\n").arg(fullLocationPackage).arg(AFILE.toLower());
    s += QString("public class A\n");
    s += ("{\n");

    for(auto iter = l.set.begin(); iter != l.set.end(); iter++)
    {
        s += QString("  public final static String %1 =\"%1\";\n").arg(*iter);
    }

    s += ("}\n");

    return s;
}

NAMESPACE_END





