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
#include <QDir>
#include "IdGenerator.h"
#include "STARTING_ODD_INVENTORY_CODE.h"
namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {


                class IFile : public IdGenerator
                {
                    mutable QString maxFileSeg;
                    mutable QString package;
                    mutable QString sceneFolder;

                public:
                    IFile()
                            : IdGenerator(STARTING_ODD_INVENTORY_CODE){}
                    IFile(QString sceneFolder, QString maxFileSeg, QString package)
                            : IdGenerator(STARTING_ODD_INVENTORY_CODE)
                            , maxFileSeg(maxFileSeg)
                            , package(package)
                            , sceneFolder(sceneFolder)
                    {
                    }

                    int getNumberOfEntries()
                    {
                        int number = mapOfObjectNames.size();
                        return number;
                    }

                    ~IFile()
                    {
                        //   writeToFile();
                    }
                private:
                    bool writeToFile(QString find, QString replace)
                    {
                        sceneFolder = sceneFolder.replace(find, replace);
                        package = package.replace(find,replace);

                        QDir dir;
                        dir.mkpath(sceneFolder);
                        QFile file(sceneFolder+"\\i.java");
                        if (!file.open(QFile::WriteOnly | QFile::Truncate)) 
                            return false;


                        QTextStream f(&file);
                        f << ("package "+package+"." + maxFileSeg + ";\n");
                        f << ("\n");
                        f << ("public interface i\n");
                        f << ("{\n");
                        f << ("    public enum names");
                        f << ("    {\n");
                        QMap<QString, int>::iterator iter = mapOfObjectNames.begin();
                        for(;iter!=mapOfObjectNames.end();iter++)
                        {
                            f << "        " << (iter.key().toUpper()) << ",\n";
                        }
                        f << ("    }\n");

                        iter =mapOfObjectNames.begin();
                        for(;iter!=mapOfObjectNames.end();iter++)
                        {
                            f << ("    public static final int ") <<  (iter.key().toUpper()) << " = " << (iter.value()) << (";\n");
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
