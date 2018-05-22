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
#include <allcaps\NAMESPACE_BEGIN.h>
#include <allcaps\NAMESPACE_END.h>
NAMESPACE_BEGIN
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
        path = path.toLower();

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

    QStringList getSubFiles(QString fullFolderPath) const
    {
        return getFilesForTrueFoldersForFalse(fullFolderPath,true);
    }

    QStringList getSubFolders(QString fullFolderPath) const
    {
        return getFilesForTrueFoldersForFalse(fullFolderPath,false);
    }

    QStringList getFilesForTrueFoldersForFalse(QString fullFolderPath,bool isLookingForFiles) const
    {
        fullFolderPath.replace("\\","/");
        fullFolderPath = fullFolderPath.toLower();
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
                if(item.contains(fullFolderPath+"/"))
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
    QString getItem(int i){ return m_list.at(i); }
    void dump()
    {
        for(int i=0;i<getNumberOfItems();i++)
        {
            qDebug(" %s ",getItem(i).toUtf8().data());

        }
    }
};
NAMESPACE_END