#pragma once
#ifndef __ZCE_BITS_H__
#define __ZCE_BITS_H__

template<typename T>
inline unsigned zce_bits_count(T v)
{
    unsigned long c; // c accumulates the total bits set in v
    for (c = 0; v; c++)
        v &= v - 1; // clear the least significant bit set
    return c;
}

#endif //
