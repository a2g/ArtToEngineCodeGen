/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
*/
#pragma once

static QString GetRealObjectSeg(QString objectSeg)
{
   // remove the prefix ie "_80_"
    QString realObjectSeg = objectSeg.mid(4);
    return realObjectSeg;
}

