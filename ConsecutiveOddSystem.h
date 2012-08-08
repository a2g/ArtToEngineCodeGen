/*  Copyright 2011 Anthony Cassidy
* Usage of the works is permitted provided that this instrument is retained with the works, so that any entity that uses the works is notified of this instrument.
*  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
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
