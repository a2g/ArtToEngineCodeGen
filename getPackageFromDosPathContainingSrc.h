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
#include "SRC.h"

inline QString getPackageFromDosPathContainingSrc(const QString& subFolderContainingSRC)
{
        QString subFolder = subFolderContainingSRC.toLower();
        QString src = QString(SRC).toLower();
        int findSRC = subFolder.lastIndexOf(src);

        int startOfCOM = findSRC + src.size() + 1;
        QString package = subFolder.mid(startOfCOM);
        package.replace('/','.');
        return package;
}
                                        
