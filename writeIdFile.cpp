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

#include "WriteIdFile.h"
#include <DomLocation.h>
#include "gtest/gtest.h"

using namespace com::github::a2g::generator;
namespace  //anonymous for linker errors
{
    const char* EXPECTED =
        "package com.visuals.inventories.canoeinv;\n"                                                                                                                                       "\n"
        "public class I\n"
        "{\n"
        "    public enum Enum\n"
        "    {\n"
        "        BEAVER(101),\n"
        "        COW(102),\n"
        "        NECKLACE_L(103),\n"
        "        NECKLACE_S(104),\n"
        "        WALRUS(105),\n"
        "        ;\n"
        "        Enum(int code) {\n"
        "        }\n"
        "    }\n"
        "    public static Enum getEnum(int value)\n"
        "    {\n"
        "        switch(value)\n"
        "        {\n"
        "        case 101: return Enum.BEAVER;\n"
        "        case 102: return Enum.COW;\n"
        "        case 103: return Enum.NECKLACE_L;\n"
        "        case 104: return Enum.NECKLACE_S;\n"
        "        case 105: return Enum.WALRUS;\n"
        "        };\n"
        "        return null;\n"
        "    }\n"
        "    public static final short BEAVER = 101;\n"
        "    public static final short COW = 102;\n"
        "    public static final short NECKLACE_L = 103;\n"
        "    public static final short NECKLACE_S = 104;\n"
        "    public static final short WALRUS = 105;\n"
        "}\n";


    TEST(WriteIFileTest, Try)
    {
        // arrange
        Dom2Location l;
        l.fullLocationPackage = "com.visuals.inventories";

        Dom2Loader& c = l.addNewLoader("canoeInv");

        c.addNewId("BEAVER",101);
        c.addNewId("COW", 102);
        c.addNewId("NECKLACE_L", 103);
        c.addNewId("NECKLACE_S", 104);
        c.addNewId("WALRUS" ,105);

        QString s = com::github::a2g::generator::writeIdFile(c);

        QStringList actual = s.split("\n");
        QStringList expected = QString(EXPECTED).split("\n");

        auto size = std::min(actual.size(), expected.size());

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