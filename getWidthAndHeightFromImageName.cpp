/*
 * Copyright 2018 Anthony Cassidy
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

#include "gtest/gtest.h"
#include "GetWidthAndHeightFromImageName.h"
using namespace com::github::a2g;


TEST(GetWidthAndHeightFromImageNameTest, AnimationFormatJan2016)
{
    // arrange
    // act
    QString imageName("e:/conanx/xappswing/xsrc/xcom/sourceimages/bird/_70_bird_main/_90_hatchlings/big_bite/_123x456_orig_0000.png");
    QPoint p = generator::getWidthAndHeightFromImageName(imageName);

    // assert
    ASSERT_EQ(123, p.x());
    ASSERT_EQ(456, p.y());
}

TEST(GetWidthAndHeightFromImageNameTest, InventoryFormatJan2016)
{
    // arrange
    // act
    QString invImageName("_64x32_rope_.png");
    QPoint p = generator::getWidthAndHeightFromImageName(invImageName);

    // assert
    ASSERT_EQ(64, p.x());
    ASSERT_EQ(32, p.y());
}

TEST(GetWidthAndHeightFromImageNameTest, TestTheDefaultByUsingInvalidImageSignature)
{
    // arrange
    // act
    QString invalid("deliberately_invalid");
    QPoint p = generator::getWidthAndHeightFromImageName(invalid);

    // assert
    ASSERT_EQ(320, p.x());
    ASSERT_EQ(180, p.y());
}


