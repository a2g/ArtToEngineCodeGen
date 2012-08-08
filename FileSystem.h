/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
*/
#pragma once
#include <QStringList>
#include <QDir>
namespace com
{ 
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class FileSystem
                {
                private:
                    QStringList m_list;
                public:
                    void clear()
                    {
                        m_list.clear();
                    }

                    void add(QString path)
                    {
                        // make sure all the paths we are adding are forward-slashed (not back-slashed)
                        path.replace("\\","/");

                        int numberOfSlashes = path.count("/")+1;
                        for(int i=0;i<numberOfSlashes;i++)
                        {
                            m_list.push_back(path);
                            QDir dir(path);
                            int j = path.lastIndexOf(dir.dirName());
                            path.truncate(j-1);
                        }
                        m_list.removeDuplicates();
                        m_list.sort();
                    }
                    
                    QStringList getSubFiles(QString fullFolderPath)
                    {
                        return getFilesForTrueFoldersForFalse(fullFolderPath, true);
                    }

                    QStringList getSubFolders(QString fullFolderPath)
                    {
                        return getFilesForTrueFoldersForFalse(fullFolderPath, false);
                    }

                    QStringList getFilesForTrueFoldersForFalse(QString fullFolderPath, bool isLookingForFiles)
                    {
                        fullFolderPath.replace("\\", "/");
                        bool isLookingForFolders = !isLookingForFiles;
                        int numberOfForwardSlashesNeeded = fullFolderPath.count("/") + 1;
                        QStringList toReturn;
                        int count = m_list.count();
                        for(int i=0;i<count;i++)
                        {
                            QString item = m_list[i];
                            int numberOfForwardSlashes = item.count("/");
                            if(numberOfForwardSlashes==numberOfForwardSlashesNeeded)
                            {
                                if(item.contains(fullFolderPath)&&item != fullFolderPath)
                                {
                                    bool isFile = item.contains(".");
                                    bool isFolder = !isFile;
                                    if((isFile & isLookingForFiles) || (isFolder & isLookingForFolders))
                                    {
                                        toReturn.push_back(item);                             
                                    }
                                }
                            }
                        }
                        return toReturn;
                    }
                    
                    int getNumberOfItems(){ return m_list.count(); }
                    QString getItem(int i){ return m_list.at(i);}
                    void populateFromRealSystem(QString rootOfRealFileSystem){}
                };
            }
        }
    }
}
