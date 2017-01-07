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
#include "IdGenerator.h"
#include "STARTING_ODD_OBJECTS_CODE.h"

namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class OFile : public IdGenerator
                {

                private:
                    mutable QString package;
                    mutable QString m_sceneSeg;
                    mutable QString m_sceneFolder;

                public:
                    OFile()
                            : IdGenerator(STARTING_ODD_OBJECTS_CODE)
                    {
                    }


                    OFile(const QString& package, QString maxFileSeg, QString sceneFolder)
                            :   IdGenerator(STARTING_ODD_OBJECTS_CODE)
                            , package(package)
                            , m_sceneSeg( maxFileSeg)
                            , m_sceneFolder( sceneFolder)
                    {
                        
                    }

                    int getNumberOfEntries()
                    {
                        int number = mapOfObjectNames.size();
                        return number;
                    }

                    ~OFile()
                    {
                        //   writeToFile();
                    }
                private:
                    bool writeToFile(QString find, QString replace)
                    {
                        m_sceneFolder = m_sceneFolder.replace(find, replace);
                        package = package.replace(find,replace);

                        QDir dir;
                        dir.mkpath(m_sceneFolder);
                        QFile file(m_sceneFolder+"\\O.java");
                        if (!file.open(QFile::WriteOnly | QFile::Truncate)) 
                            return false;


                        QTextStream f(&file);
                        f << ("package "+package+"." + m_sceneSeg + ";\n");
                        f << ("\n");
                        f << ("public class O\n");
                        f << ("{\n");
                        f << ("     public enum Enum");
                        f << ("     {\n");
                        QMap<QString, int>::iterator iter = mapOfObjectNames.begin();
                        for(;iter!=mapOfObjectNames.end();iter++)
                        {
                            f<<"        " << (iter.key().toUpper()) << "((short)" << (iter.value()) <<"),\n";
                        }
                        f << ("        ;\n");
                        f << ("        //private final short code;\n");
                        f << ("        Enum(short code) {\n");
                        f << ("           // this.code = code;\n");
                        f << ("        }\n");
                        f << ("    }\n");
                        f << ("    public static Enum getEnum(int value)\n");
                        f << ("    {                               \n");
                        f << ("        switch(value)               \n");
                        f << ("        {                           \n");
                        iter = mapOfObjectNames.begin();
                        for(;iter!=mapOfObjectNames.end();iter++)
                        {
                            f << "            case " << (iter.value()) << ": return Enum." + (iter.key().toUpper()) <<";\n";
                        }
                        f << ("        };\n");
                        f << ("        return null;\n");
                        f << ("    }\n");

                        iter = mapOfObjectNames.begin();
                        for(;iter!=mapOfObjectNames.end();iter++)
                        {
                            f << ("    public static final short ") <<  (iter.key().toUpper()) << " = " << (iter.value()) << (";\n");
                        }

                        f << ("}\n");
                        file.close();
                        return true;

                    }

                    friend class OutputFiles;


                };
            }
        }
    }
}
