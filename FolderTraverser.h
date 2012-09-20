/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
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

                    void SearchForRootOfResourcesAndGenerateIfFound(QString startingPath);
                private:
                    void SearchRecursively(QString subfolder, QString startTargetFolder, QString endTargetFolder);
                    void Generate(QString rootFolder, QString package);
                };
            }
        }
    }
}
        
