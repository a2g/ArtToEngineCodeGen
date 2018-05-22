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
#include <memory>
#include "allcaps\NAMESPACE_BEGIN.h"
#include "allcaps\NAMESPACE_END.h"
#include "Dom2Location.h"
#include "allcaps\INITIALS.h"
#include "DomLocation.h"
#include "IdGenerator.h"
#include "getRealObjectSeg.h"
#include "getObjectPlusAnim.h"

NAMESPACE_BEGIN
static std::auto_ptr<Dom2Location> buildDom2(const DomLocation& location)
{
    QString bad = "?"; //this is a char that will invalidate a filename, and thus any XFile class if used as param1

    std::auto_ptr<Dom2Location> toReturn;
    toReturn.reset(new Dom2Location());
    Dom2Location& location2 = toReturn->asRef();
    location2.locationPath = location.locationPath;
    location2.fullLocationPackage = location.fullLocationPackage;
    
    for(int i = 0; i < location.getCameraCount(); i++)
    {
        auto c = location.getCameraAt(i);
        auto cameraSeg = c.cameraSeg.toLower();
        Dom2Loader& loader = location2.addNewLoader(cameraSeg);


        if(cameraSeg.toLower().contains("inv"))
        {
            loader.type = LoaderTypeEnum::INVENTORY;
            IdGenerator ids(StartingOddEnum::STARTING_ODD_INVENTORY_CODE);
            for(int i=0;i<c.getObjectCount();i++)
            {
                const DomObject& o = c.getObjectAt(i);
                auto objectName = getRealObjectSeg(o.objectSeg, true).toUpper();
                loader.addNewId(objectName,ids.getIdForName(objectName));
            }
  
        }
        else if (!cameraSeg.toLower().contains("shared"))
        {
            loader.type = LoaderTypeEnum::NORMAL;
            IdGenerator ids(StartingOddEnum::STARTING_ODD_OBJECTS_CODE);
            for(int i=0;i<c.getObjectCount();i++)
            {
                const DomObject& o = c.getObjectAt(i);
                auto objectName = getRealObjectSeg(o.objectSeg,false).toUpper();
                int id = ids.getIdForName(objectName);
                loader.addNewId(objectName,id);
            }
        }
        else
        {
            loader.type = LoaderTypeEnum::SHARED;
        }
       

        Dom2Bundle& initialsBundle = loader.addNewBundle(INITIALS);
        bool isInv = loader.type == LoaderTypeEnum::INVENTORY;
        bool isShared = loader.type == LoaderTypeEnum::SHARED;

        auto objectSize = c.getObjectCount();
        for(int j = 0; j < objectSize; j++)
        {
            auto o = c.getObjectAt(j);
            auto realObjectSegToUpper = getRealObjectSeg(o.objectSeg,loader.type==LoaderTypeEnum::INVENTORY).toUpper();

            for(int k = 0; k < o.getAnimationCount(); k++)
            {
                auto a = o.getAnimationAt(k);
                auto objectPlusAnimToUpper = getObjectPlusAnim(o.objectSeg,a.animSeg).toUpper();
                if(loader.type!=LoaderTypeEnum::INVENTORY)
                    location2.addAnimCode(objectPlusAnimToUpper);
                auto seg = a.animSeg.toUpper();
                
                Dom2Bundle& theBundle =  isInv || (!isShared &&( seg=="INITIAL" || seg =="PLACEHOLDER"|| seg=="BLANK"))? initialsBundle : loader.addNewBundle(objectPlusAnimToUpper.toUtf8().data());

                for(int l = 0; l < a.getFrameCount();l++)
                {
                    const DomFrame& f = a.getFrameAt(l);
                    
                    int x = f.getRect().x();
                    int y = f.getRect().y();
                    int w = f.getRect().width();
                    int h = f.getRect().height();
                    QString animSeg = a.animSeg;
                    QString animSegtoUpper = animSeg.toUpper();

                    theBundle.addFrame(o.prefix,x,y,w,h,realObjectSegToUpper,animSegtoUpper, f.getPngSaveTo());
                }
            }

            if(initialsBundle.getFrameCount()==0)
            {
                loader.removeBundle(INITIALS);
            }
        }
    }
    return toReturn;
}
NAMESPACE_END