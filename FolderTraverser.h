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
#include "FileSystem.h"
#include "OutputFiles.h"
namespace com
{ 
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class FolderTraverser
                {
                    FileSystem& files;
                    OutputFiles& output;
                public:
                    FolderTraverser(FileSystem& fs, OutputFiles& output)
                        : files(fs)
                        , output(output)
                    {  }

                    void generateFilesFromSourceFolderOrASubFolderThereof(QString startingPath);
                private:                    
                    void searchRecursively(QString folder, QString targetPathSegment);
                    void generate(QString rootFolder, QString package);
                    void processAllSubFolders(QString folder);
                    void processJustThisSubFolder(QString subFolder);

                };
            }
        }
    }
}
        
