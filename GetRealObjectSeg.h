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

static QString getRealObjectSeg(QString objectSeg, bool isInv)
{
    // inventory doesn't have a concept of objectSeg...
    // so for the inventory case we'll get called here with the containing
    // folder of an inventory object which is a resolution eg "32x64" etc.
    // we don't want to use that at all, so we just return "_".
    if(isInv)
        return "_";

   // remove the prefix ie "_80_"
    QString realObjectSeg = objectSeg.mid(4);
    return realObjectSeg;
}

