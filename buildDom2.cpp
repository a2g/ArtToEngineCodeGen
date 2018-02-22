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

#include "buildDom.h"
#include "gtest/gtest.h"
using namespace com::github::a2g::generator;
#include "DomLocation.h"
#include "Dom2Location.h"
#include "buildDom2.h"
namespace 
{
    TEST(buildDomTest,Inventory)
    {
        // arrange
        DomLocation l;
        l.fullLocationPackage = "com.visuals";
        l.locationSeg = "canoeLoc";
        {
            auto c1 = l.addNewCamera("canoeMain").addNewObject("_40_WATERFALL").addNewAnimation("PLACEHOLDER").addFrame("blah/blah1.png");
        }
        {
            auto c2 = l.addNewCamera("canoeShared").addNewObject("_40_WATERFALL").addNewAnimation("INITIAL").addFrame("blah/blah1.png");
        }

        // act
        auto returnVal = buildDom2(l);
        auto location = returnVal->asRef();

        //assert
        ASSERT_EQ(2,location.getLoaderCount());

        auto& loaderMain = location.getLoaderAt(0);
        auto& loaderShared = location.getLoaderAt(1);
        
        ASSERT_STREQ("canoemain",loaderMain.loaderSeg.toUtf8().data());
        ASSERT_STREQ("canoeshared",loaderShared.loaderSeg.toUtf8().data());

        auto& bundleShar = loaderShared.getBundles().at(1);
        auto& bundleMain = loaderMain.getBundles().at(0);

        ASSERT_EQ(1, loaderMain.getBundleCount());
        ASSERT_EQ(2, loaderShared.getBundleCount());
   
        ASSERT_STREQ("INITIALS",loaderMain.getBundles().at(0).bundleName.toUtf8().data());//initials contains WATERFALL_PLACEHOLDER
        ASSERT_STREQ("INITIALS",           loaderShared.getBundles().at(0).bundleName.toUtf8().data());//initials contains WATERFALL_PLACEHOLDER

        ASSERT_STREQ("WATERFALL_INITIAL",  loaderShared.getBundles().at(1).bundleName.toUtf8().data());
        

        ASSERT_EQ(1, loaderMain.getBundles().at(0).getFrameCount());
        ASSERT_EQ(0, loaderShared.getBundles().at(0).getFrameCount());
        ASSERT_EQ(1, loaderShared.getBundles().at(1).getFrameCount());

    }

}
