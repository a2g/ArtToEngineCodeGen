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
                            : IdGenerator(ConsecutiveOddSystem::STARTING_INVENTORY_SEED){}
                    IFile(QString sceneFolder, QString maxFileSeg, QString package)
                            : IdGenerator(ConsecutiveOddSystem::STARTING_INVENTORY_SEED)
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
                        f << ("import com.github.a2g.core.interfaces.InventoryItemEnumAPI;\n");
                        f << ("public enum a\n");
                        f << ("implements InventoryItemEnumAPI\n");
                        f << ("{\n");

                        QMap<QString, int>::iterator iter = mapOfObjectNames.begin();
                        for(;iter!=mapOfObjectNames.end();iter++)
                        {
                            f << (iter.key().toUpper()) << "(" << (iter.value()) << ("),\n");
                        }		
			f << (";\n");
                        f << ("private int value;\n");

                        f << ("a(int value)\n");
                        f << ("{\n");
                        f << ("		this.value = value;\n");
                        f << ("	}\n");

                        f << ("@Override\n");
                        f << ("public int getValue() {\n");
                        f << ("  return value;\n");
                        f << ("}\n");
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






