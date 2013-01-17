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
                    mutable QString m_sceneFolder;
                    mutable QString m_sceneSeg;
                    mutable QString package;

                public:
                    OFile()
                         : IdGenerator(ConsecutiveOddSystem::STARTING_LOCAL_SEED)
                    {
                    }


                    OFile(QString sceneFolder, QString maxFileSeg, const QString& package)
                        : IdGenerator(ConsecutiveOddSystem::STARTING_LOCAL_SEED)
                        , m_sceneFolder( sceneFolder)
                        , m_sceneSeg( maxFileSeg)
                        , package(package)
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
                        QFile file(m_sceneFolder+"\\o.java");
                        if (!file.open(QFile::WriteOnly | QFile::Truncate)) 
                            return false;

                        QTextStream f(&file);
                        f << ("package "+package+"." + m_sceneSeg + ";\n");
                        f << ("\n");
                        f << ("public class o\n");
                        f << ("{\n");

                        QMap<QString, int>::iterator iter = mapOfObjectNames.begin();
                        for(;iter!=mapOfObjectNames.end();iter++)
                        {
                            f << ("  public static final short ") <<  (iter.key().toUpper()) << " = " << (iter.value()) << (";\n");
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
