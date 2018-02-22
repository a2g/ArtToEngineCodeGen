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

namespace {
    /*
    TEST(buildDomTest,FlatList)
    {
        // arrange
        FileSystem files;
        files.add("c:/src/blah/blah/sourceimages/mission/_00_Animations/a.java");
        files.add("c:/src/blah/blah/sourceimages/mission/camera/_10_bg/initial/a.png");
        files.add("c:/src/blah/blah/sourceimages/mission/camera/_10_fg/initial/a.png");
        files.add("c:/src/blah/blah/sourceimages/mission/camera/_10_man/initial/cropped_orig_0000.png");
        files.add("c:/src/blah/blah/sourceimages/mission/shared_man/_10_man/talk/cropped_orig_0000.png");
        files.add("c:/src/blah/blah/sourceimages/mission/shared_man/_10_man/talk/cropped_orig_0001.png");
        files.add("c:/src/blah/blah/sourceimages/mission/shared_man/_10_man/talk/cropped_orig_0002.png");

        // act
        auto returnVal = buildDom("c:/src/blah/blah/sourceimages/mission",files);
        auto location = returnVal->asRef();

        // assert
        ASSERT_STREQ("C:/SRC/BLAH/BLAH/SOURCEIMAGES/MISSION",location.locationPath.toLocal8Bit());// ensure its upper case
        ASSERT_EQ(2,location.getCameraCount());

        ASSERT_STREQ("mission",location.locationSeg.toLocal8Bit());
        ASSERT_EQ(2,location.getCameraCount());

        ASSERT_STREQ("camera",location.getCameraAt(0).cameraSeg.toLocal8Bit());
        int numberOfObjects = location.getCameraAt(0).getObjectCount();
        ASSERT_EQ(3, numberOfObjects);

        ASSERT_STREQ("_10_man",location.getCameraAt(0).getObjectAt(0).objectSeg.toLocal8Bit());
        ASSERT_STREQ("_10_fg",location.getCameraAt(0).getObjectAt(1).objectSeg.toLocal8Bit());
        ASSERT_STREQ("_10_bg",location.getCameraAt(0).getObjectAt(2).objectSeg.toLocal8Bit());

        ASSERT_STREQ("initial",location.getCameraAt(0).getObjectAt(0).getAnimationAt(0).animSeg.toLocal8Bit());

        ASSERT_STREQ("shared_man",location.getCameraAt(1).cameraSeg.toLocal8Bit());
    }
    */

    TEST(buildDomTest,Inventory)
    {
        // arrange
        FileSystem files;
        files.add("c:/src/blah/blah/sourceimages/mission/camerainv/apple/a.png");
        files.add("c:/src/blah/blah/sourceimages/mission/camerainv/onion/b.png");
        files.add("c:/src/blah/blah/sourceimages/mission/camerainv/mango/c.png");

        // act
        auto returnVal = buildDom("c:/src/blah/blah/sourceimages/mission",files);
        auto location = returnVal->asRef();

        // assert
        ASSERT_STREQ("C:/SRC/BLAH/BLAH/SOURCEIMAGES/MISSION",location.locationPath.toLocal8Bit());// ensure its upper case
        ASSERT_STREQ("mission",location.locationSeg.toLocal8Bit());

        ASSERT_EQ(1,location.getCameraCount());
        ASSERT_STREQ("camerainv",location.getCameraAt(0).cameraSeg.toLocal8Bit());

        int numberOfObjects = location.getCameraAt(0).getObjectCount();
        ASSERT_EQ(3,numberOfObjects);
        ASSERT_STREQ("onion",location.getCameraAt(0).getObjectAt(0).objectSeg.toUtf8().data());
        ASSERT_STREQ("mango",location.getCameraAt(0).getObjectAt(1).objectSeg.toUtf8().data());
        ASSERT_STREQ("apple",location.getCameraAt(0).getObjectAt(2).objectSeg.toUtf8().data());
        ASSERT_STREQ("b.png", location.getCameraAt(0).getObjectAt(0).getAnimationAt(0).getFrameAt(0).getPngSaveTo());
        ASSERT_STREQ("c.png", location.getCameraAt(0).getObjectAt(1).getAnimationAt(0).getFrameAt(0).getPngSaveTo());
        ASSERT_STREQ("a.png", location.getCameraAt(0).getObjectAt(2).getAnimationAt(0).getFrameAt(0).getPngSaveTo());

      //  ASSERT_STREQ("initial",location.getCameraAt(0).getObjectAt(0).getAnimationAt(0).animSeg.toLocal8Bit());
      
       // ASSERT_STREQ("shared_man",location.getCameraAt(1).cameraSeg.toLocal8Bit());
    }

}
