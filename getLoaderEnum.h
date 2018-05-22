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
#include "IsShared.h"
#include "IsInventory.h"

QString getLoaderEnum(QString cameraSeg)
{
    if(com::github::a2g::generator::IsInventory(cameraSeg))
        return "2";
    else if(com::github::a2g::generator::IsShared(cameraSeg))
        return "1";
    else return "0";
}
