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
#include <WriteSwingBundle.h>
#include <gtest/gtest.h>
#include <DomLocation.h>

using namespace com::github::a2g::generator;
namespace  //anonymous for linker errors
{
    const char* EXPECTED =
        "       package com.visuals.canoe.canoe;\n"
        "\n"
        "   import com.github.a2g.core.interfaces.internal.IMasterPresenterFromBundle;\n"
        "   import com.github.a2g.core.platforms.swing.dependencies.PackagedImageForSwing;\n"
        "   import com.google.gwt.event.dom.client.LoadHandler;\n"
        "\n"
        "   public class WATERFALL_FALLING\n"
        "   {\n"
        "       public class MyRes{\n"
        "           public static final MyRes RESOURCE = new MyRes();\n"
        "           public String waterfall_falling_0(){ return \"blah/blah.png\"; }\n"
        "           public String waterfall_falling_1(){ return \"blah/blah2.png\"; }\n"
        "           public String waterfall_falling_2(){ return \"blah/blah3.png\"; }\n"
        "       }\n"
        "       public static boolean addImage(IMasterPresenterFromBundle api, LoadHandler lh, int i)\n"
        "       {\n"
        "           final MyRes res = MyRes.RESOURCE;\n"
        "           if (i == -1) {}\n"
        "           else if (i == 0) return api.addImageForASceneObject(lh, -858993460,0,0,0,0, \"WATERFALL\",\"FALLING\",(short)0,\"WATERFALL_FALLING\").arg(new PackagedImageForSwing(res.waterfall_falling_0()));\n"
        "           else if (i == 1) return api.addImageForASceneObject(lh, -858993460,0,0,0,0, \"WATERFALL\",\"FALLING\",(short)1,\"WATERFALL_FALLING\").arg(new PackagedImageForSwing(res.waterfall_falling_1()));\n"
        "           else if (i == 2) return api.addImageForASceneObject(lh, -858993460,0,0,0,0, \"WATERFALL\",\"FALLING\",(short)2,\"WATERFALL_FALLING\").arg(new PackagedImageForSwing(res.waterfall_falling_2()));\n"
        "           return false;\n"
        "       }\n"
        "   }";



    TEST(WriteSwingBundleTest, Try)
    {
        // arrange
        Dom2Location location;
        location.locationPath = "c:/src/blah/blah/sourceimages";
        location.fullLocationPackage = "com.visuals.locationSeg";
        Dom2Loader& inv = location.addNewLoader("cameraInv");
        Dom2Loader& sha = location.addNewLoader("cameraShared");
        Dom2Loader& nml = location.addNewLoader("normal");

        Dom2Bundle& bun = nml.addNewBundle("MAN_WALKING").addFrame(10,0,0,90,54,"MAN","WALKING","blah.png");
        nml.addNewBundle("WOMAN_WALKING").addFrame(10,0,0,90,54,"WOMAN","WALKING","blah.png");
        nml.addNewBundle("WOMAN_WALKING").addFrame(10,0,0,90,54,"WOMAN","WALKING","blah.png");
        inv.addNewBundle("InvBundle").addFrame(10,0,0,90,54,"GUN","WALKING","blah.png");
        sha.addNewBundle("MAN_JUMPING").addFrame(10,0,0,90,54,"MAN","WALKING","blah.png");


        std::string temp = com::github::a2g::generator::writeSwingBundle(bun).toUtf8().data();

        QStringList actual = QString(temp.c_str()).split("\n");
        QStringList expected = QString(EXPECTED).split("\n");

        //ASSERT_LT(expected.size(), actual.size());
        auto size = std::min(expected.size(), actual.size());
        for (int i = 0; i <  size; i++)
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
