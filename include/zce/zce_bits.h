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

//most significant bit
//int msb(unsigned x) {
//    union { double a; int b[2]; };
//    a = x;
//    return (b[1] >> 20) - 1023;
//}
template<typename T>
inline unsigned zce_bits_msb_index(T v) {
    unsigned target_level = 0;
    while (v >>= 1) ++target_level;
    return target_level;
}

#endif //
