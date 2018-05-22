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
#include "buildFilenamesAndContents.h"
#include "DomLocation.h"
#include "gtest/gtest.h"
#include "InscriberNull.h"
#include <ostream>
using namespace com::github::a2g::generator;
using namespace std;
#ifdef NDEBUG
class DebugStream {};
template <typename T>
DebugStream &operator<<(DebugStream &s,T) { return s; }
#else
typedef ostream DebugStream;
#endif

DomLocation location;
namespace {

    class FileSystemTest: public ::testing::Test
    {
    protected:

    public:
        FileSystemTest() {
            // You can do set-up work for each test here.
        }

        virtual ~FileSystemTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp()
        {



        }

        virtual void TearDown()
        {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }
    };

    // Tests that the Foo::Bar() method does Abc.

    TEST(buildFilenamesAndcontentsTest,test1)
    {
        // arrange
        Dom2Location location;
        location.locationPath = "c:/src/blah/blah/sourceimages";
        location.fullLocationPackage = "com.visuals.locationSeg";
        Dom2Loader& inv = location.addNewLoader("cameraInv");
        Dom2Loader& sha = location.addNewLoader("cameraShared");
        Dom2Loader& nml = location.addNewLoader("normal");

        nml.addNewBundle("MAN_WALKING").addFrame(10,0,0,90,54,"MAN","WALKING", "blah.png");
        nml.addNewBundle("WOMAN_WALKING").addFrame(10,0,0,90,54,"WOMAN","WALKING","blah.png");
        nml.addNewBundle("WOMAN_WALKING").addFrame(10,0,0,90,54,"WOMAN","WALKING","blah.png");
        inv.addNewBundle("InvBundle").addFrame(10,0,0,90,54,"GUN","WALKING","blah.png");
        sha.addNewBundle("MAN_JUMPING").addFrame(10,0,0,90,54,"MAN","WALKING","blah.png");

        int i = nml.getBundleCount();
        int j = location.getLoaderCount();
        int a = location.getLoaderAt(0).getBundleCount();
        int b = location.getLoaderAt(1).getBundleCount();
        int c = location.getLoaderAt(2).getBundleCount();
        InscriberNull Inscriber;
        auto files = buildFilenamesAndContents(location,Inscriber);
        for(int i=0;i<files.size();i++)
        {
            cerr << files.at(i).second <<"\n";
        }
        // act
        int size = files.size();
        auto e00 = files.at(0).second;
        auto e01 = files.at(1).second;
        auto e02 = files.at(2).second;
        auto e03 = files.at(3).second;
        auto e04 = files.at(4).second;
        auto e05 = files.at(5).second;
        auto e06 = files.at(6).second;

        auto e07 = files.at(7).second;
        auto e08 = files.at(8).second;
        auto e09 = files.at(9).second;
        auto e10 = files.at(10).second;
        /*
        auto e11 = files.at(11).second;
        auto e12 = files.at(12).second;
        auto e13 = files.at(13).second;
        auto e14 = files.at(14).second;
        auto e15 = files.at(15).second;
        auto e16 = files.at(16).second;
        auto e17 = files.at(17).second;
        auto e18 = files.at(18).second;
        auto e19 = files.at(19).second;
        auto e20 = files.at(20).second;
        */
        // assert

        // ASSERT_EQ(7, size);

        ASSERT_STRCASEEQ("c:/src/blah/blah/sourceimages/_00_animations/A.java",e00.c_str());
        ASSERT_STRCASEEQ("c:/src/blah/blah/sourceimages/cameraInv/InvBundle.java",e01.c_str());
        ASSERT_STRCASEEQ("c:/src/blah/blah/sourceimages/cameraInv/Loader.java",e02.c_str());
        ASSERT_STRCASEEQ("c:/src/blah/blah/sourceimages/cameraInv/I.java",e03.c_str());
        ASSERT_STRCASEEQ("c:/src/blah/blah/sourceimages/cameraShared/MAN_JUMPING.java",e04.c_str());
        ASSERT_STRCASEEQ("c:/src/blah/blah/sourceimages/cameraShared/Loader.java",e05.c_str());
        ASSERT_STRCASEEQ("c:/src/blah/blah/sourceimages/cameraShared/I.java",e06.c_str());
        ASSERT_STRCASEEQ("c:/src/blah/blah/sourceimages/normal/MAN_WALKING.java",e07.c_str());
        ASSERT_STRCASEEQ("c:/src/blah/blah/sourceimages/normal/WOMAN_WALKING.java",e08.c_str());
        ASSERT_STRCASEEQ("c:/src/blah/blah/sourceimages/normal/Loader.java",e09.c_str());
        ASSERT_STRCASEEQ("c:/src/blah/blah/sourceimages/normal/I.java",e10.c_str());
    }


}
