/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
*/
#pragma once
#include "GetRealObjectSeg.h"



static QString GetObjectPlusAnim(QString objectSeg, QString animSeg)
{
    QString objectPlusAnim = GetRealObjectSeg(objectSeg) + "_" + animSeg;
    return objectPlusAnim;
}
                                        