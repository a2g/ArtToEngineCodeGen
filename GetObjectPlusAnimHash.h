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
#include <time.h>
#include <locale>
 static int getObjectPlusAnimHash(QString objectSeg, QString animSeg)
 {
    std::locale loc;
    const std::collate<char>& coll = std::use_facet<std::collate<char> >(loc);

    std::string objectPlusAnim = getObjectPlusAnim(objectSeg,animSeg).toStdString();
    int idForObjPlusAnim  = coll.hash(objectPlusAnim.data(), objectPlusAnim.data() + objectPlusAnim.length());
    return idForObjPlusAnim;
 }
