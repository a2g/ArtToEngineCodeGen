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

#include "WriteInitialEnumFile.h"
#include <DomLocation.h>
#include "gtest/gtest.h"

using namespace com::github::a2g::generator;
namespace  //anonymous for linker errors
{


    TEST(WriteInitialEnumFileTest, Try)
    {
        // arrange
        // arrange
        Dom2Location location;
        location.locationPath = "c:/src/blah/blah/sourceimages";
        location.fullLocationPackage = "com.visuals.canoe";
        
        location.addAnimCode("MAN_INITIAL");
        location.addAnimCode("MAN_WALKING");
        location.addAnimCode("WOMAN_INITIAL");
        location.addAnimCode("WOMAN_WALKING");

        std::string s = com::github::a2g::generator::writeAFile(location).toUtf8().data();

        const char* EXPECTED =
            "package com.visuals.canoe._00_animations;\n"
            "public class A\n"
            "{\n"
            "    public final static String WOMAN_WALKING =\"WOMAN_WALKING\";\n"
            "    public final static String WOMAN_INITIAL =\"WOMAN_INITIAL\";\n"
            "    public final static String MAN_WALKING =\"MAN_WALKING\";\n"
            "    public final static String MAN_INITIAL=\"MAN_INTIIAL\";\n"
            "}";


        QStringList actual = QString(s.c_str()).split("\n");
        QStringList expected = QString(EXPECTED).split("\n");



        auto size = std::min(actual.size(),expected.size());

        for (int i = 0; i < size; i++)
        {
            auto actualTrimmed = actual[i].trimmed().toStdString();
            auto expectedTrimmed = expected[i].trimmed().toStdString();
            if (actualTrimmed == expectedTrimmed)
            {
                ASSERT_EQ(expectedTrimmed, actualTrimmed);
            }
            else
            {
                ASSERT_EQ(expectedTrimmed, actualTrimmed);
            }

        }
        return;
    }
}