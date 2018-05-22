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

#include "WriteLoader.h"
#include <DomLocation.h>
#include "gtest/gtest.h"

using namespace com::github::a2g::generator;
namespace  //anonymous for linker errors
{
    const char* EXPECTED =
        "package com.visuals.canoe.canoe;\n"
        "\n"
        "import com.github.a2g.core.interfaces.IOnEnqueueResources;\n"
        "import com.github.a2g.core.interfaces.internal.IBundleLoader;\n"
        "import com.github.a2g.core.interfaces.internal.ISingleBundle;\n"
        "import com.github.a2g.core.interfaces.internal.IMasterPresenterFromBundle;\n"
        "import com.google.gwt.event.dom.client.LoadHandler;\n"
        "import com.google.gwt.core.client.GWT;\n"
        "import com.google.gwt.core.client.RunAsyncCallback;\n"
        "import com.google.gwt.core.client.Scheduler;\n"
        "\n"
        "public class Loader implements IBundleLoader\n"
        "{\n"
        "    @Override\n"
        "    public int getImageWidth() { return 640; }\n"
        "    @Override\n"
        "    public int getImageHeight() { return 360; }\n"
        "    @Override\n"
        "    public int getLoaderEnum() { return 0; }\n"
        "    \n"
        "    @Override\n"
        "    public int loadImageBundle(final LoadHandler lh, final IMasterPresenterFromBundle api, final int chosenByIndex, final String chosenByName)\n"
        "    {\n"
        "        if (chosenByIndex == -2) {}\n"
        "        else if (chosenByIndex == 0 ||chosenByName==\"WATERFALL_FALLING\") {\n"
        "            return WATERFALL_FALLING.loadImageBundle(lh,api);\n"
        "        }\n"
        "        else if (chosenByIndex == 1 ||chosenByName==\"MAN_WALKING\") {\n"
        "            return MAN_WALKING.loadImageBundle(lh,api);\n"
        "        }\n"
        "        else if (chosenByIndex == 2 ||chosenByName==\"BEAVER_EATING\") {\n"
        "            return BEAVER_EATING.loadImageBundle(lh,api);\n"
        "        }\n"
        "        return 0;\n"
        "    }\n"
        "    @Override\n"
        "    public int getNumberOfBundles() { return 3; }";



    TEST(WriteLoaderTest, Try)
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


        std::string blah = com::github::a2g::generator::writeLoader(nml, false).toUtf8().data();

        QStringList actual = QString(blah.c_str()).split("\n");
        QStringList expected = QString(EXPECTED).split("\n");

        int size = std::min(actual.size(), expected.size());
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