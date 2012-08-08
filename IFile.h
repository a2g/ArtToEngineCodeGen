/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
*/
#pragma once

#include <QFile>
#include <QTextStream>
#include <QMap>
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
                    IFile(const QString& sceneFolder, const QString& maxFileSeg, const QString& package) 
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
                    bool writeToFile()
                    {
                        QFile file(sceneFolder+"\\i.java");
                        if (!file.open(QFile::WriteOnly | QFile::Truncate)) 
                            return false;


                        QTextStream f(&file);
                        f << ("package "+package+"." + maxFileSeg + ";\n");
                        f << ("\n");
                        f << ("public class i\n");
                        f << ("{\n");

                        QMap<QString, int>::iterator iter = mapOfObjectNames.begin();
                        for(;iter!=mapOfObjectNames.end();iter++)
                        {
                            f << ("  public static final int ") <<  (iter.key().toUpper()) << " = " << (iter.value()) << (";\n");
                        }		

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

