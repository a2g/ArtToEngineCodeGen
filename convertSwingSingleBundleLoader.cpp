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

#include <convertSwingSingleBundleLoader.h>
#include <writeSwingClass.h>
#include <DomLocation.h>
#include "gtest/gtest.h"


using namespace com::github::a2g::generator;
namespace  //anonymous for linker errors
{
    const char* EXPECTED =
        "public static ISingleBundle VINES2_PLACEHOLDER(){return new Loader_().new VINES2_PLACEHOLDER();}\n"
        "public class VINES2_PLACEHOLDER implements ISingleBundle\n"
        "{\n"
        "    public VINES2_PLACEHOLDER() {}\n"
        "    @Override public int getBundleNumber() { return 0; }\n"
        "    @Override public String toString() { return \"VINES2_PLACEHOLDER\"; }\n"
        "    @Override public int getSize() { return 1; }\n"
        "    @Override public int loadImageBundle(final LoadHandler lh, final IMasterPresenterFromBundle api)\n"
        "    {\n"
        "        GWT.runAsync\n"
        "        (\n"
        "            new RunAsyncCallback()\n"
        "        {\n"
        "            public void onSuccess()\n"
        "        {\n"
        "            // Schedule the IncrementalCommand instance to run when\n"
        "            // control returns to the event loop by returning 'true'\n"
        "            Scheduler.get().scheduleFixedDelay\n"
        "            (\n"
        "                new Scheduler.RepeatingCommand()\n"
        "            {\n"
        "                int counter=0;\n"
        "                @Override\n"
        "                    public boolean execute()\n"
        "                {\n"
        "                    com.visuals.canoe.canoe.VINES2_PLACEHOLDER.addImage(api, lh, counter);\n"
        "                    return (++counter<getSize());\n"
        "                }\n"
        "            }\n"
        "            ,0\n"
        "                );\n"
        "        }\n"
        "        public void onFailure(Throwable caught)\n"
        "        {\n"
        "            lh.onLoad(null);\n"
        "        }\n"
        "      }\n"
        "    );\n"
        "    return 0;\n"
        "  }\n"
        "}\n"; //skip last line in expected, puresly so we can use less than in the test

    TEST(WriteGwtSingleBundleLoaderTest,Try)
    {
        QString s1 = com::github::a2g::generator::writeSwingClass("com.visuals.canoeLoc.canoeCam","SubLoader");
        QString s = com::github::a2g::generator::convertSwingSingleBundleLoader(s);

        QStringList actual = s.split("\n");
        QStringList expected = QString(EXPECTED).split("\n");

        ASSERT_EQ(expected.size(),actual.size());

        for(int i = 0; i < expected.size(); i++)
        {
            auto actualTrimmed = actual[i].trimmed().toStdString();
            auto expectedTrimmed = expected[i].trimmed().toStdString();
            if(actualTrimmed == expectedTrimmed)
            {
                ASSERT_EQ(expectedTrimmed,actualTrimmed);
            }
            else
            {
                ASSERT_EQ(expectedTrimmed,actualTrimmed);
            }

        }
        return;
    }
}