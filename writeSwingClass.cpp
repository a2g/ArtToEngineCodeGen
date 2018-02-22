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

#include "WriteSwingClass.h"
#include "DomLocation.h"
#include "gtest/gtest.h"

using namespace com::github::a2g::generator;
namespace  //anonymous for linker errors
{
    const char* EXPECTED =
        "public static ISingleBundle WATERFALL_FALLING(){return new Loader().new WATERFALL_FALLING();}\n"
        "public class WATERFALL_FALLING implements ISingleBundle\n"
        "{\n"
        "    Timer timer;\n"
        "    public WATERFALL_FALLING() {}\n"
        "    @Override public int getBundleNumber() { return 0; }\n"
        "    @Override public String toString() { return \"WATERFALL_FALLING\"; }\n"
        "    @Override public int getSize() { return 1; }\n"
        "    @Override public int loadImageBundle(final LoadHandler lh, final IMasterPresenterFromBundle api)\n"
        "    {\n"
        "        timer = new Timer\n"
        "        (\n"
        "            0\n"
        "            , new ActionListener()\n"
        "            {\n"
        "            int counter = 0;\n"
        "        @Override\n"
        "            public void actionPerformed(ActionEvent e)\n"
        "        {\n"
        "            com.visuals.canoeLoc.canoeCam.WATERFALL_FALLING.addImage(api, lh, counter + 0);\n"
        "            counter++;\n"
        "            if (counter == getSize())\n"
        "                timer.stop();\n"
        "            lh.onLoad(null);\n"
        "        }\n"
        "        }\n"
        "        );\n"
        "        timer.setInitialDelay(0);\n"
        "        timer.start();\n"
        "        return 0;\n"
        "    }\n"
        "}\n";


    TEST(WriteGwtSingleBundleLoaderTest, Try)
    {
        // arrange

        // act
        QString s = com::github::a2g::generator::writeSwingClass("com.visuals.canoeLoc.canoeCam", "WATERFALL_FALLING");


        // assert
        QStringList actual = s.split("\n");
        QStringList expected = QString(EXPECTED).split("\n");
        auto size = std::min(actual.size(), expected.size());
        for (int i = 0; i < expected.size(); i++)
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