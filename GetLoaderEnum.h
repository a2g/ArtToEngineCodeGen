#pragma once

/* This is needed to construct the package path to put at the top of generated java code.
 * At first, I was searching for COM., since all my paths started with "com."
 * But then I realised that some people have other TLDs eg "org." and some don't
 * use reverse domain name convention at all.
 * In Gwt projects, "src" is the standard location of source code. So this is better.
 *
 */
#include "IsShared.h"
#include "IsInventory.h"

QString GetLoaderEnum(QString cameraSeg)
{
    if(com::github::a2g::generator::IsInventory(cameraSeg))
        return "2";
    else if(com::github::a2g::generator::IsShared(cameraSeg))
        return "1";
    else return "0";
}
