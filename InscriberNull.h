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
#include "DomLocation.h"
#include "IInscriber.h"
#include "allcaps\NAMESPACE_BEGIN.h"
#include "allcaps\NAMESPACE_END.h"
NAMESPACE_BEGIN

class InscriberNull: public IInscriber
{
    QString writeAFile(const Dom2Location& location) const override{ return "";}
    QString writeInitialEnumFile(const Dom2Loader& loader) const override{ return ""; }
    QString writeIFile(const Dom2Loader& camera) const override{ return "";}
    QString writeOFile(const Dom2Loader& camera) const override{ return "";}
    QString writeLoader(const Dom2Loader& loader) const override{ return "";}
    QString writeBundle(const Dom2Bundle& bundle) const override{ return "";}
    QString writeLoaderClass(const QString& package,const QString& className,const QString& loaderSeg, int numberOfImages,QPoint res,int type) const override{ return "";}
};

NAMESPACE_END