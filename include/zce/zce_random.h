#pragma once

#include <limits.h>
#include <zce/zce_config.h>

class ZCE_API zce_random
{
    static const int MBIG = 2147483647;
    static const int MSEED = 161803398;
    static const int MZ = 0;

    int inext, inextp;
    int SeedArray[56];

    void init(int Seed);

    int internal_sample();

    double sample();

    double get_sample_for_large_range();

public:

    zce_random();

    zce_random(int Seed);

    /*=====================================Next=====================================
    **Returns: An int [0..Int32.MaxValue)
    **Arguments: None
    **Exceptions: None.
    ==============================================================================*/
    int next();

    /*=====================================Next=====================================
    **Returns: An int [minvalue..maxvalue)
    **Arguments: minValue -- the least legal value for the Random number.
    **           maxValue -- One greater than the greatest legal return value.
    **Exceptions: None.
    ==============================================================================*/
    int next(int minValue, int maxValue);


    /*=====================================Next=====================================
    **Returns: An int [0..maxValue)
    **Arguments: maxValue -- One more than the greatest legal return value.
    **Exceptions: None.
    ==============================================================================*/
    int next(int maxValue);

    /*=====================================Next=====================================
    **Returns: A double [0..1)
    **Arguments: None
    **Exceptions: None
    ==============================================================================*/
    double nextdouble();
};