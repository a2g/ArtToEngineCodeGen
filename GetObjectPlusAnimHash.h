/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
*/
#pragma once
#include <time.h>
#include <locale>
 static int GetObjectPlusAnimHash(QString objectSeg, QString animSeg)
 {
    std::locale loc;
    const std::collate<char>& coll = std::use_facet<std::collate<char> >(loc);

    std::string objectPlusAnim = GetObjectPlusAnim(objectSeg,animSeg).toStdString();
    int idForObjPlusAnim  = coll.hash(objectPlusAnim.data(), objectPlusAnim.data() + objectPlusAnim.length());
    return idForObjPlusAnim;
 }
