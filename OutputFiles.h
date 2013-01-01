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
#include <QStringList>
#include <QDir>
#include "AFile.h"
#include "OFile.h"
#include "IFile.h"
#include "LoaderAndResFilePair.h"
namespace com
{ 
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class OutputFiles
                {
                private:
                    QVector<AFile> allAFiles;
                    QVector<IFile> allIFiles;
                    QVector<OFile> allOFiles;
                    QVector<LoaderAndResFilePair> allOnlys;
                    QVector<LoaderAndResFilePair> allFirsts;
                    QVector<LoaderAndResFilePair> allLasts;

                public:
                    OutputFiles(){}

                    QVector<AFile> getAFiles(){ return allAFiles; }
                    QVector<IFile> getIFiles(){ return allIFiles; }
                    QVector<OFile> getOFiles(){ return allOFiles; }
                    QVector<LoaderAndResFilePair> getOnlys(){return allOnlys; }
                    QVector<LoaderAndResFilePair> getFirsts(){return allFirsts; }
                    QVector<LoaderAndResFilePair> getLasts(){return allLasts; }

                    void addAFile(AFile file)
                    {
                        allAFiles.push_back(file);
                    }
                    void addIFile(IFile file)
                    {
                        allIFiles.push_back(file);
                    }
                    void addOFile(OFile file)
                    {
                        allOFiles.push_back(file);
                    }
                    void addFirst(LoaderAndResFilePair file)
                    {
                        allFirsts.push_back(file);
                    }
                    void addOnly(LoaderAndResFilePair file)
                    {
                        allOnlys.push_back(file);
                    }
                    void addLast(LoaderAndResFilePair file)
                    {
                        allLasts.push_back(file);
                    }

                    void writeAll(QString find, QString replace, bool isGwt)
                    {

                        for(int i=0;i<allAFiles.count();i++)
                        {
                            allAFiles[i].deleteMasterFolder(find, replace);
                            allAFiles[i].writeToFile(find , replace);
                        }
                        for(int i=0;i<allIFiles.count();i++)
                        {
                            allIFiles[i].writeToFile(find , replace);
                        }
                        for(int i=0;i<allOFiles.count();i++)
                        {
                            allOFiles[i].writeToFile(find, replace);
                        }
                        for(int i=0;i<allOnlys.count();i++)
                        {
                            allOnlys[i].writeToFile(find, replace, isGwt);
                        }
                        for(int i=0;i<allFirsts.count();i++)
                        {
                            allFirsts[i].writeToFile(find , replace, isGwt);
                        }
                        for(int i=0;i<allLasts.count();i++)
                        {
                            allLasts[i].writeToFile(find, replace, isGwt);
                        }
                    }
                };
            }
        }
    }
}
