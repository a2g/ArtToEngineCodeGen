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

namespace com
{ 
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class FileSystemFolder
                {
                    FileSystem& fileSystem;
                    QString fullFolderPath;
                public:
                    FileSystemFolder(QString fullFolderPath)
                    {
                        this->fullFolderPath = fullFolderPath;
                    }

                    bool isValid()
                    {
                        bool isValid = fileSystem.Contains(folder);
                        return isValid;
                    }

                    std::string GetAllPathSegments()
                    {
                        return fullFolderPath;
                    }
                    std::string GetEndSegment()
                    {
                        QDir dir(fullFolderPath);
                        QString endSegment = dir.dirName();
                        return endSegment;
                    }
					int GetNumberOfSubFolders()
                    {
                        int numberOfSubFolders = fileSystem.GetNumberOfSubFoldersInFolder(fullFolderPath);
                    }
					FileSystemFolder GetSubFolder(int i)
                    {
                        String s = fileSystem.GetSubFolderOfFolder(i, fullFolderPath);
                        return s;
                    }
					int GetNumberOfFiles()
                    {
                        int numberOfFiles = fileSystem.GetNumberOfFilesInFolder(fullFolderPath);
                        return numberOfFiles;
                    }
					std::string GetFile(int i)
                    {
                        String s = fileSystem.GetFileOfFolder(i, fullFolderPath);
                        return s;
                    }
                };
            }
        }
    }
}