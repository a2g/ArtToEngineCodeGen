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
#include <convertBundleToGwt.h>
#include <writeBundle.h>

#include <gtest/gtest.h>
#include <DomLocation.h>

using namespace com::github::a2g::generator;
namespace  //anonymous for linker errors
{
    const char* EXPECTED =
        "package com.visuals.canoeLoc.canoeCam;\n"
        "\n"
        "    import com.github.a2g.core.platforms.html4.PackagedImageForHtml4;\n"
        "    import com.github.a2g.core.interfaces.internal.IMasterPresenterFromBundle;\n"
        "    import com.google.gwt.resources.client.ClientBundle;\n"
        "    import com.google.gwt.event.dom.client.LoadHandler;\n"
        "    import com.google.gwt.core.client.GWT;\n"
        "    import com.google.gwt.resources.client.ImageResource;\n"
        "\n"
        "    public class WATERFALL_FALLING\n"
        "    {\n"
        "        public static int getSize() { return 2; }\n"
        "        public interface MyRes extends ClientBundle {\n"
        "            public static final WATERFALL_FALLING.MyRes RESOURCE = GWT.create(WATERFALL_FALLING.MyRes.class);\n"
        "            @Source(\"blah/blah.png\")\n"
        "            abstract ImageResource waterfall__falling_80();\n"
        "            @Source(\"blah/blah.png\")\n"
        "            abstract ImageResource waterfall__falling_81();\n"
        "        }\n"
        "        public static boolean addImage(IMasterPresenterFromBundle api, LoadHandler lh, int i)\n"
        "        {\n"
        "            final MyRes res = MyRes.RESOURCE;\n"
        "            if (i == -1) {}\n"
        "            else if (i == 0) return api.addImageForASceneObject(lh, 20, 11289, 245, 231, 115, \"WATERFALL\", \"FALLING\", (short)1919, \"WATERFALL_FALLING\", new PackagedImageForHtml4(res.waterfall__falling_80()));\n"
        "            else if (i == 1) return api.addImageForASceneObject(lh, 20, 11291, 245, 229, 115, \"WATERFALL\", \"FALLING\", (short)1919, \"WATERFALL_FALLING\", new PackagedImageForHtml4(res.waterfall__falling_81()));\n"
        "            return false;\n"
        "        }\n"
        "    }\n";


    TEST(WriteGwtBundleTest,Try)
    {
        // arrange
        DomLocation l;
        l.fullLocationPackage = "com.visuals";
        l.locationSeg = "canoeLoc";

        auto c = l.addNewCamera("canoeCam");
        auto o1 = c.addNewObject("_40_WATERFALL"); 
        auto a1 = o1.addNewAnimation("FALLING"); 
        {
            DomFrame f1("blah/blah1.png");
            DomFrame f2("blah/blah2.png");
            DomFrame f3("blah/blah3.png");
            DomFrame f4("blah/blah4.png");
            a1.addFrame(f1);
            a1.addFrame(f2);
        }


        QString s1 = com::github::a2g::generator::writeBundle(a1);
        QString s = com::github::a2g::generator::convertBundleToGwt(s1);

        QStringList actual = s.split("\n");
        QStringList expected = QString(EXPECTED).split("\n");

        //ASSERT_LT(expected.size(), actual.size());

        for(int i = 0; i < expected.size(); i++)
        {
            auto actualTrimmed = actual[i].trimmed().toStdString();
            auto expectedTrimmed = expected[i].trimmed().toStdString();
            if(actualTrimmed == expectedTrimmed)
            {
                EXPECT_EQ(expectedTrimmed,actualTrimmed);
            }
            else
            {
                EXPECT_EQ(expectedTrimmed,actualTrimmed);
            }

        }
        return;
    }
}
