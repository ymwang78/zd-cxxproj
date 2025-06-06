/*
 ---------------------------------------------------------------------------
 Copyright (c) 1998-2008, Brian Gladman, Worcester, UK. All rights reserved.

 LICENSE TERMS

 The redistribution and use of this software (with or without changes)
 is allowed without the payment of fees or royalties provided that:

  1. source code distributions include the above copyright notice, this
     list of conditions and the following disclaimer;

  2. binary distributions include the above copyright notice, this list
     of conditions and the following disclaimer in their documentation;

  3. the name of the copyright holder is not used to endorse products
     built using this software without specific written permission.

 DISCLAIMER

 This software is provided 'as is' with no explicit or implied warranties
 in respect of its properties, including, but not limited to, correctness
 and/or fitness for purpose.
 ---------------------------------------------------------------------------
 Issue Date: 20/12/2007
*/

#ifndef RDTSC_H
#define RDTSC_H

#if defined( __GNUC__ )

    static inline volatile unsigned long long read_tsc(void)
    {
        unsigned long long cy;
        asm volatile("cpuid; rdtsc" : "=A" (cy));
        return cy;
    }

#elif defined( _WIN32 ) || defined( _WIN64 )

#   include <intrin.h>
#   pragma intrinsic( __rdtsc )

    __inline volatile unsigned long long read_tsc(void)
    {
        return __rdtsc();
    }

#else
#   error A high resolution timer is not available
#endif

#endif
