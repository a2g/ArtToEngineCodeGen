/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
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