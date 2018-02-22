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
#include <convertSwingLoader.h>
#include <writeSwingPerAnimationLoader.h>

#include <DomLocation.h>
#include "gtest/gtest.h"

using namespace com::github::a2g::generator;
namespace  //anonymous for linker errors
{
    const char* EXPECTED =
        "package com.visuals.canoe.canoe; "                                                                                                                                       "\n"
        ""                                                                                                                                                                        "\n"
        "   import com.github.a2g.core.interfaces.internal.IBundleLoader;"                                                                                                        "\n"
        "   import com.github.a2g.core.interfaces.internal.ISingleBundle;"                                                                                                        "\n"
        "   import com.github.a2g.core.interfaces.internal.IMasterPresenterFromBundle;"                                                                                           "\n"
        "   import com.google.gwt.event.dom.client.LoadHandler;"                                                                                                                  "\n"
        "   import com.google.gwt.core.client.GWT;"                                                                                                                               "\n"
        "   import com.google.gwt.core.client.RunAsyncCallback;"                                                                                                                  "\n"
        "   import com.google.gwt.core.client.Scheduler;"                                                                                                                         "\n"
        ""                                                                                                                                                                        "\n"
        "   public class Loader implements IBundleLoader"                                                                                                                         "\n"
        "   {"                                                                                                                                                                    "\n"
        "       @Override"                                                                                                                                                        "\n"
        "       public int getImageWidth() { return 640; }"                                                                                                                       "\n"
        "       @Override"                                                                                                                                                        "\n"
        "       public int getImageHeight() { return 360; }"                                                                                                                      "\n"
        "       @Override"                                                                                                                                                        "\n"
        "       public int getLoaderEnum() { return 0; }"                                                                                                                         "\n"
        "       @Override"                                                                                                                                                        "\n"
        "       public int getNumberOfBundles() { return 2; }"                                                                                                                    "\n"
        ""                                                                                                                                                                        "\n"
        "       @Override"                                                                                                                                                        "\n"
        "       public int loadImageBundle(final LoadHandler lh, final IMasterPresenterFromBundle api, final int chosenByIndex, final String chosenByName)"                       "\n"
        "       {"                                                                                                                                                                "\n"
        "           if (chosenByIndex == -2) {}"                                                                                                                                   "\n"
        "           else if (chosenByIndex == 0||chosenByName==\"WATERFALL_FALLING\") {"                                                                                          "\n"
        "               return WATERFALL_FALLING.loadImageBundle(lh,api);"                                                                                                        "\n"
        "           }"                                                                                                                                                            "\n"
        "           else if (chosenBundle == 1||chosenByName==\"MAN_WALKING\") {"                                                                                                 "\n"
        "               return MAN_WALKING.loadImageBundle(lh,api);"                                                                                                              "\n"
        "           }"                                                                                                                                                            "\n"
        "           return 0;"                                                                                                                                                    "\n"
        "       }";


    TEST(WriteGwtLoaderTest,Try)
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

        QString s1 = com::github::a2g::generator::writeSwingPerAnimationLoader(c);
        QString s = com::github::a2g::generator::convertSwingLoader(s1);

        QStringList actual = s.split("\n");
        QStringList expected = QString(EXPECTED).split("\n");

        ASSERT_TRUE(expected.size() < actual.size());

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