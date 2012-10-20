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

#pragma once

class ConsecutiveOddSystem
{
public:
    static const int MAXIMUM_OBJECTS = 100; // if you want a large range of consecutive odd numbers that produce unique products, then the lower bound of that range odd number needs to be sufficiently high
    static const int STARTING_ODD = 1787;// see Series with unique products. Odd numbers 1787-1987 give unique products,
    static const int STARTING_GLOBAL_SEED = 1;
    static const int STARTING_INVENTORY_SEED = 34;
    static const int STARTING_LOCAL_SEED = 67;
};
