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
#include "Dom2Location.h"
#include "gtest/gtest.h"

using namespace com::github::a2g::generator;
namespace  //anonymous for linker errors
{
    TEST(Dom2LoaderTest,EnsureThereAreNoDuplicates)
    {
        Dom2Location location;
        // arrange
        Dom2Loader loader(location);
        loader.addNewBundle("blah");
        loader.addNewBundle("blah");
        int size = loader.getBundleCount();

        ASSERT_EQ(1,size);
    }

    TEST(Dom2LoaderTest,EnsureReturnValueWorkdsUsingReference)
    {
        // arrange
        Dom2Location location;
        Dom2Loader loader(location);

        // act
        Dom2Bundle& blah =  loader.addNewBundle("blah");
        blah.bundleName = "newName";
        
        // assert
        auto result = loader.addNewBundle("blah").bundleName.toUtf8();

        ASSERT_STREQ("newName",result.data());
    }

    TEST(Dom2LoaderTest,EnsureReturnValueWorksUsingAutoReference)
    {
        // arrange
        Dom2Location location;
        Dom2Loader loader(location);

        // act
        auto& blah =  loader.addNewBundle("blah");
        blah.bundleName = "newName";

        // assert
        auto result = loader.addNewBundle("blah").bundleName.toUtf8();

        ASSERT_STREQ("newName",result.data());
    }

    TEST(Dom2LoaderTest,NotATestSoMuchAsWeKnowThisDoesntWork)
    {
        // arrange
        Dom2Location location;
        Dom2Loader loader(location);

        // act
        auto blah =  loader.addNewBundle("blah");
        blah.bundleName = "newName";

        // assert
        auto result = loader.addNewBundle("blah").bundleName.toUtf8();

        ASSERT_STREQ("BLAH",result.data());
    }
}