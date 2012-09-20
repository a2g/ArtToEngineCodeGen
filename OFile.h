/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
*/
#pragma once

#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QString>
#include "IdGenerator.h"

namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class OFile : public IdGenerator
                {

                private:
                    mutable QString m_sceneFolder;
                    mutable QString m_sceneSeg;
                    mutable QString package;

                public:
                    OFile()
                         : IdGenerator(ConsecutiveOddSystem::STARTING_LOCAL_SEED)
                    {
                    }


                    OFile(const QString& sceneFolder, QString maxFileSeg, const QString& package) 
                        : IdGenerator(ConsecutiveOddSystem::STARTING_LOCAL_SEED)
                        , m_sceneFolder( sceneFolder)
                        , m_sceneSeg( maxFileSeg)
                        , package(package)
                    {
                        
                    }

                    int getNumberOfEntries()
                    {
                        int number = mapOfObjectNames.size();
                        return number;
                    }

                    ~OFile()
                    {
                     //   writeToFile();
                    }
                private:
                    bool writeToFile()
                    {
                        QFile file(m_sceneFolder+"\\o.java");
                        if (!file.open(QFile::WriteOnly | QFile::Truncate)) 
                            return false;

                        QTextStream f(&file);
                        f << ("package "+package+"." + m_sceneSeg + ";\n");
                        f << ("\n");
                        f << ("public class o\n");
                        f << ("{\n");

                        QMap<QString, int>::iterator iter = mapOfObjectNames.begin();
                        for(;iter!=mapOfObjectNames.end();iter++)
                        {
                            f << ("  public static final short ") <<  (iter.key().toUpper()) << " = " << (iter.value()) << (";\n");
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

