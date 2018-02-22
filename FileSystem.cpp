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

#include "FileSystem.h"
#include "gtest/gtest.h"
using namespace com::github::a2g::generator;


namespace {
    FileSystem files;

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
            files.clear();
            files.add("c:/src/blah/blah/sourceimages/blah/_00_Animations/a.java");
            files.add("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_bg/initial/a.png");
            files.add("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_fg/initial/a.png");
            files.add("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_man/talk/cropped_orig_0000.png");
            files.add("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_man/talk/cropped_orig_0001.png");
            files.add("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_man/talk/cropped_orig_0002.png");
            files.add("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_man/initial/cropped_orig_0000.png");
        }

        virtual void TearDown()
        {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }
    };

    // Tests that the Foo::Bar() method does Abc.

    TEST_F(FileSystemTest,FlatList)
    {
        // arrange

        int count = files.getNumberOfItems();
        for(int i=0;i<count;i++)
        {
            //qDebug( files.getItem(i).toUtf8());
        }
        ASSERT_EQ(22,count);

        // act
        QString e00 = files.getItem(0);
        QString e01 = files.getItem(1);
        QString e02 = files.getItem(2);
        QString e03 = files.getItem(3);
        QString e04 = files.getItem(4);
        QString e05 = files.getItem(5);
        QString e06 = files.getItem(6);
        QString e07 = files.getItem(7);
        QString e08 = files.getItem(8);
        QString e09 = files.getItem(9);
        QString e10 = files.getItem(10);
        QString e11 = files.getItem(11);
        QString e12 = files.getItem(12);
        QString e13 = files.getItem(13);
        QString e14 = files.getItem(14);
        QString e15 = files.getItem(15);
        QString e16 = files.getItem(16);
        QString e17 = files.getItem(17);
        QString e18 = files.getItem(18);
        QString e19 = files.getItem(19);
        QString e20 = files.getItem(20);

        // assert
        EXPECT_EQ(22,count);
        EXPECT_STRCASEEQ("c:",e00.toUtf8());
        EXPECT_STRCASEEQ("c:/src",e01.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah",e02.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah",e03.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages",e04.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah",e05.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_Animations",e06.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_Animations/a.java",e07.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation",e08.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_bg",e09.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_bg/initial",e10.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_bg/initial/a.png",e11.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_fg",e12.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_fg/initial",e13.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_fg/initial/a.png",e14.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_man",e15.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_man/initial",e16.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_man/initial/cropped_orig_0000.png",e17.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_man/talk",e18.toUtf8());
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_man/talk/cropped_orig_0000.png",e19.toUtf8());

    }

    TEST_F(FileSystemTest,MethodBarDoesAbc)
    {
        // assert
        QString adir = "c:";
        int acount = files.getSubFolders(adir).count();
        ASSERT_EQ(1,acount);
        EXPECT_STRCASEEQ("c:",adir.toUtf8());

        QString bdir = files.getSubFolders(adir).at(0);
        int bcount = files.getSubFolders(bdir).count();
        ASSERT_EQ(1,bcount);
        EXPECT_STRCASEEQ("c:/src",bdir.toUtf8());

        QString cdir = files.getSubFolders(bdir).at(0);
        int ccount = files.getSubFolders(cdir).count();
        ASSERT_EQ(1,ccount);
        EXPECT_STRCASEEQ("c:/src/blah",cdir.toUtf8());

        QString ddir = files.getSubFolders(cdir).at(0);
        int dcount = files.getSubFolders(ddir).count();
        ASSERT_EQ(1,dcount);
        EXPECT_STRCASEEQ("c:/src/blah/blah",ddir.toUtf8());

        QString edir = files.getSubFolders(ddir).at(0);
        int ecount = files.getSubFolders(edir).count();
        ASSERT_EQ(1,ecount);
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages",edir.toUtf8());

        QString fdir = files.getSubFolders(edir).at(0);
        int fcount = files.getSubFolders(fdir).count();
        ASSERT_EQ(2,fcount);
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah",fdir.toUtf8());

        QString gdir = files.getSubFolders(fdir).at(1);
        int gcount = files.getSubFolders(gdir).count();
        ASSERT_EQ(3,gcount);
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation",gdir.toUtf8());


        QString hdir = files.getSubFolders(gdir).at(2);
        int hcount = files.getSubFolders(hdir).count();
        ASSERT_EQ(2,hcount);
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_man",hdir.toUtf8());

        QString idir = files.getSubFolders(hdir).at(1);
        int icount = files.getSubFiles(idir).count();
        ASSERT_EQ(3,icount);
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_man/talk",idir.toUtf8());

        QString jfile = files.getSubFiles(idir).at(2);
        EXPECT_STRCASEEQ("c:/src/blah/blah/sourceimages/blah/_00_riverjunction_animation/_10_man/talk/cropped_orig_0002.png",jfile.toUtf8());
    }
}
