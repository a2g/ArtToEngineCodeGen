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
#include <QRegularExpression.h>
#include "DomAnimation.h"
#include "allcaps\NAMESPACE_BEGIN.h"
#include "allcaps\NAMESPACE_END.h"
#include "allcaps\SRC.h"
NAMESPACE_BEGIN
static QString convertBundleToGwt(const QString& swingBundle)
{
    QString s;
    QString className;
    
    auto lines = swingBundle.split("\n");
    for(int i = 0; i < lines.size(); i++)
    {
        auto line = lines[i].trimmed(); 
        if(line=="import com.github.a2g.core.platforms.swing.dependencies.PlatformPackagedImageForSwing;")
        {
            s+="import com.google.gwt.resources.client.ClientBundle;\n";
            s+="import com.google.gwt.resources.client.ImageResource;\n";
            s+="import com.google.gwt.core.client.GWT;\n";
            s+="import com.github.a2g.core.platforms.html4.dependencies.PlatformPackagedImageForHtml4;\n";
        }
        else if(line.startsWith("public class"))
        {
            s+=line+"\n";
            className = line;
            className.replace("public class","");
            className = className.trimmed();
        }
        else if(line=="public static class MyRes{")
        {
            s+="public interface MyRes extends ClientBundle\n{\n";
        }
        else if(line=="public static final MyRes RESOURCE = new MyRes();")
        {
            s+= QString("public static final %1.MyRes RESOURCE =  GWT.create(%1.MyRes.class);\n").arg(className);
        }
        else if(line==";}")
        {
            s+="\n";
        }
        else if(line.startsWith("public String"))
        {
            line.replace("public String ","");
            int bracket = line.indexOf("(");
            QString resourceName = line.left(bracket);
            int src = line.indexOf(SRC.toLower());
            int ext = line.indexOf(".");
            QString path = line.mid(src+4,ext);//the +4 is to get past src/, and end up at com/
            s+=QString("@Source(\"%1)\n").arg(path);
            s+=QString("abstract ImageResource %1();\n").arg(resourceName);
        }
        else if(line.startsWith("else if"))
        {
            line.replace("PackagedImageForSwing","PackagedImageForHtml4");
            s+=line+"\n";
        }
        else
        {
            s+=line+"\n";
        }

    }

    return s;
}
NAMESPACE_END