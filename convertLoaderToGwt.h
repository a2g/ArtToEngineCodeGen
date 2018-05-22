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
#include <QVector.h>
#include "DomAnimation.h"
#include "allcaps\NAMESPACE_BEGIN.h"
#include "allcaps\NAMESPACE_END.h"
NAMESPACE_BEGIN

static QString convertLoaderToGwt(const QString& swingLoader)
{
    QString s;
    
    auto lines = swingLoader.split("\n");
    for(int i = 0; i < lines.size(); i++)
    {
        auto line = lines[i].trimmed();
        if(line.contains("import javax.swing.Timer"))
        {
        
            s+="import com.google.gwt.core.client.Scheduler;\n";
        }
        else if(line.contains("java.awt.event.ActionListener"))
        {
            s+="import com.google.gwt.core.client.GWT;\n";
        }
        else if(line.contains("java.awt.event.ActionEvent"))
        {
            s+="import com.google.gwt.core.client.RunAsyncCallback;\n";
        }
        else
        {
            s+=line+"\n";
        }
    }
    return s;
}
NAMESPACE_END
