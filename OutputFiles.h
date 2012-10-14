/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
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
                    OutputFiles(){};

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
