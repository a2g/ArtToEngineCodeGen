/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
*/
#pragma once

#include <QFile>
#include <QTextStream>
#include <QMap>
#include "ConsecutiveOddSystem.h"

namespace com
{
    namespace github
    {
        namespace a2g
        {
            namespace generator
            {
                class IdGenerator
                {
                protected:
                    QMap<QString, int> mapOfObjectNames;
                    int currentSeed;
                public:
                    IdGenerator(int seed)
                            :currentSeed(seed)
                    {
                    }

                    int getIdForName(QString objectName)
                    {
                        if(mapOfObjectNames.find(objectName)!=mapOfObjectNames.end())
                            return mapOfObjectNames[objectName];
                        //else we've never seen it before
                        int id = ConsecutiveOddSystem::STARTING_ODD + ( 2*currentSeed++);
                        mapOfObjectNames[objectName] = id;
                        return id;
                    }
                };
            }
        }
    }
}






