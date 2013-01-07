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
#include <QDir>
#include <QTextStream>
#include <QMap>
#include <QString>
namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class AFile 
                {
                    mutable QString m_currentPath;
                    mutable QString m_animFolder;
                    mutable QString package;
                    QMap<QString,int> mapOfAnims;
                public:
                    AFile(){}
                    AFile(const QString& currentPath, const QString& animFolder, const QString& package) 
                            : m_currentPath(currentPath)
                            , m_animFolder(animFolder)
                            , package(package)
                    {
                    }
                    int getNumberOfEntries()
                    {
                        int number = mapOfAnims.size();
                        return number;
                    }
                    void insert(std::string objectPlusAnim,  int idForObjPlusAnim)
                    {
                        mapOfAnims.insert(QString::fromStdString(objectPlusAnim), idForObjPlusAnim);
                    }
                    ~AFile()
                    {
                        // writeToFile();
                    }
                private://to prevent unit test to add
                    void deleteMasterFolder(QString find, QString replace)
                    {
                        m_currentPath = m_currentPath.replace(find, replace);
                        package = package.replace(find,replace);

                        QDir dir(package);
                        bool wasDeleted = dir.rmdir(package);
                        wasDeleted = wasDeleted;
                    }
                    bool writeToFile(QString find, QString replace)
                    {
                        m_currentPath = m_currentPath.replace(find, replace);
                        package = package.replace(find,replace);

                        QDir dir;
                        dir.mkpath(m_currentPath+"/" + m_animFolder);
                        QFile file(m_currentPath+"/" + m_animFolder + "/a.java");
                        if (!file.open(QFile::WriteOnly | QFile::Truncate)) 
                            return false;

                        QTextStream f(&file);
                        f << ("package " + package + "._00_animations;\n");
                        f << ("import com.github.a2g.core.interfaces.AnimationEnumAPI;\n");
                        f << ("public enum a\n");
                        f << ("implements AnimationEnumAPI\n");
                        f << ("{\n");

                        for(QMap<QString,int>::iterator iter = mapOfAnims.begin();iter!=mapOfAnims.end();iter++)
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








