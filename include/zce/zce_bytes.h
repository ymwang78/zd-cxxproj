#pragma once
// ***************************************************************
//  zce_bytes   version:  1.0   -  date: 2003/01/01
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This is a part of ZCE lib, which inherited from ubeda/utiny.
//  Copyright (C) 2002 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __zce_bytes_h__
#define __zce_bytes_h__

#ifndef __i386__
#   ifdef _WIN32
#       define __i386__
#       define ZBYTE_LITTLE_ENDIAN
#   endif
#endif //__i386__

#ifdef __i386__
#    define ZBYTE_CAREFULE_ALLIGN 0
#endif

#ifndef ZBYTE_CAREFULE_ALLIGN
#   define ZBYTE_CAREFULE_ALLIGN 1
#endif

#if !defined(ZBYTE_LITTLE_ENDIAN) && !defined(ZBYTE_BIG_ENDIAN)
#  define ZBYTE_LITTLE_ENDIAN
#endif

#define ZBYTE_IS_LITTLE_ENDIAN 'ABCD' == 0x41424344

#define ZBYTE_CVAL(buf,pos)       (((const unsigned char*)(buf))[pos])
#define ZBYTE_CVAL_NC(buf,pos)    (((unsigned char*)(buf))[pos]) /* Non-const version of CVAL */
#define ZBYTE_PVAL(buf,pos)       (ZBYTE_CVAL(buf,pos))
#define ZBYTE_SCVAL(buf,pos,val)  (ZBYTE_CVAL_NC(buf,pos) = (val))

#if ZBYTE_CAREFULE_ALLIGN

#define ZBYTE_SVAL(buf,pos)       (ZBYTE_PVAL(buf,pos)|ZBYTE_PVAL(buf,(pos)+1)<<8)
#define ZBYTE_IVAL(buf,pos)       (ZBYTE_SVAL(buf,pos)|ZBYTE_SVAL(buf,(pos)+2)<<16)
#define ZBYTE_LVAL(buf,pos)       (((long long)ZBYTE_IVAL(buf,pos)) | (((long long)ZBYTE_IVAL(buf,(pos)+4))<<32))
#define ZBYTE_SSVALX(buf,pos,val) (ZBYTE_CVAL_NC(buf,pos)=(unsigned char)((val)&0xFF),ZBYTE_CVAL_NC(buf,pos+1)=(unsigned char)((val)>>8))
#define ZBYTE_SIVALX(buf,pos,val) (ZBYTE_SSVALX(buf,pos,val&0xFFFF),ZBYTE_SSVALX(buf,pos+2,val>>16))
#define ZBYTE_SLVALX(buf,pos,val) (ZBYTE_SIVALX(buf,pos,val&0xFFFFFFFF),ZBYTE_SIVALX(buf,pos+4,val>>32))
#define ZBYTE_SVALS(buf,pos)      ((short)ZBYTE_SVAL(buf,pos))
#define ZBYTE_IVALS(buf,pos)      ((long)ZBYTE_IVAL(buf,pos))
#define ZBYTE_LVALS(buf,pos)      ((long long)ZBYTE_LVAL(buf,pos))
#define ZBYTE_SSVAL(buf,pos,val)  ZBYTE_SSVALX((buf),(pos),((unsigned short)(val)))
#define ZBYTE_SIVAL(buf,pos,val)  ZBYTE_SIVALX((buf),(pos),((unsigned long)(val)))
#define ZBYTE_SLVAL(buf,pos,val)  ZBYTE_SLVALX((buf),(pos),((unsigned long long)(val)))
#define ZBYTE_SSVALS(buf,pos,val) ZBYTE_SSVALX((buf),(pos),((short)(val)))
#define ZBYTE_SIVALS(buf,pos,val) ZBYTE_SIVALX((buf),(pos),((long)(val)))
#define ZBYTE_SLVALS(buf,pos,val) ZBYTE_SLVALX((buf),(pos),((long long)(val)))

#else /* ZBYTE_CAREFULE_ALLIGN */

#define ZBYTE_SVAL(buf,pos)       (*(const unsigned short*)((const char*)(buf) + (pos)))
#define ZBYTE_SVAL_NC(buf,pos)    (*(unsigned short*)((char*)(buf) + (pos))) /* Non const version of above. */
#define ZBYTE_IVAL(buf,pos)       (*(const unsigned long*)((const char*)(buf) + (pos)))
#define ZBYTE_IVAL_NC(buf,pos)    (*(unsigned long*)((char*)(buf) + (pos))) /* Non const version of above. */
#define ZBYTE_LVAL(buf,pos)       (*(const unsigned long long*)((const char*)(buf) + (pos)))
#define ZBYTE_LVAL_NC(buf,pos)    (*(unsigned long long*)((char*)(buf) + (pos))) /* Non const version of above. */
#define ZBYTE_SVALS(buf,pos)      (*(const short*)((const char*)(buf) + (pos)))
#define ZBYTE_SVALS_NC(buf,pos)   (*(short*)((char *)(buf) + (pos))) /* Non const version of above. */
#define ZBYTE_IVALS(buf,pos)      (*(const long*)((const char*)(buf) + (pos)))
#define ZBYTE_IVALS_NC(buf,pos)   (*(long*)((char*)(buf) + (pos))) /* Non const version of above. */
#define ZBYTE_LVALS(buf,pos)      (*(const long long*)((const char*)(buf) + (pos)))
#define ZBYTE_LVALS_NC(buf,pos)   (*(long long*)((char*)(buf) + (pos))) /* Non const version of above. */

#define ZBYTE_SSVAL(buf,pos,val)  ZBYTE_SVAL_NC(buf,pos)=((unsigned short)(val))
#define ZBYTE_SIVAL(buf,pos,val)  ZBYTE_IVAL_NC(buf,pos)=((unsigned long)(val))
#define ZBYTE_SLVAL(buf,pos,val)  ZBYTE_LVAL_NC(buf,pos)=((unsigned long long)(val))
#define ZBYTE_SSVALS(buf,pos,val) ZBYTE_SVALS_NC(buf,pos)=((short)(val))
#define ZBYTE_SIVALS(buf,pos,val) ZBYTE_IVALS_NC(buf,pos)=((long)(val))
#define ZBYTE_SLVALS(buf,pos,val) ZBYTE_LVALS_NC(buf,pos)=((long long)(val))

#endif /* ZBYTE_CAREFULE_ALLIGN */

#define ZBYTE_SREV(x) ((((x)&0xFF)<<8) | (((x)>>8)&0xFF))
#define ZBYTE_IREV(x) ((ZBYTE_SREV(x)<<16) | (ZBYTE_SREV((x)>>16)))
#define ZBYTE_LREV(x) ((((long long)ZBYTE_IREV((long)x)) << 32) | ((unsigned long long)ZBYTE_IREV(((long long)x)>>32)))

#define ZBYTE_RSVAL(buf,pos)            ZBYTE_SREV(ZBYTE_SVAL(buf,pos))
#define ZBYTE_RSVALS(buf,pos)           ZBYTE_SREV(ZBYTE_SVALS(buf,pos))
#define ZBYTE_RIVAL(buf,pos)            ZBYTE_IREV(ZBYTE_IVAL(buf,pos))
#define ZBYTE_RIVALS(buf,pos)           ZBYTE_IREV(ZBYTE_IVALS(buf,pos))
#define ZBYTE_RLVAL(buf,pos)            ZBYTE_LREV(ZBYTE_LVAL(buf,pos))
#define ZBYTE_RLVALS(buf,pos)           ZBYTE_LREV(ZBYTE_LVALS(buf,pos))
#define ZBYTE_RSSVAL(buf,pos,val)       ZBYTE_SSVAL (buf,pos,ZBYTE_SREV(val))
#define ZBYTE_RSSVALS(buf,pos,val)      ZBYTE_SSVALS(buf,pos,ZBYTE_SREV(val))
#define ZBYTE_RSIVAL(buf,pos,val)       ZBYTE_SIVAL (buf,pos,ZBYTE_IREV(val))
#define ZBYTE_RSIVALS(buf,pos,val)      ZBYTE_SIVALS(buf,pos,ZBYTE_IREV(val))
#define ZBYTE_RSLVAL(buf,pos,val)       ZBYTE_SLVAL (buf,pos,ZBYTE_LREV(val))
#define ZBYTE_RSLVALS(buf,pos,val)      ZBYTE_SLVALS(buf,pos,ZBYTE_LREV(val))

/* pointer difference macro */
#define ZBYTE_PTR_DIFF(p1,p2) ((ptrdiff_t)(((const char*)(p1)) - (const char*)(p2)))
/* Alignment macros. */
#define ZBYTE_PTR_ALIGN8(p,base) ((p) + ((8 - (ZBYTE_PTR_DIFF((p), (base)) & 7)) & 7))
#define ZBYTE_PTR_ALIGN4(p,base) ((p) + ((4 - (ZBYTE_PTR_DIFF((p), (base)) & 3)) & 3))
#define ZBYTE_PTR_ALIGN2(p,base) ((p) + ((2 - (ZBYTE_PTR_DIFF((p), (base)) & 1)) & 1))
#define ZBYTE_LEN_ALIGN8(BYTELEN) ((BYTELEN + 7) & (~7))
#define ZBYTE_LEN_ALIGN4(BYTELEN) ((BYTELEN + 3) & (~3))
#define ZBYTE_LEN_ALIGN2(BYTELEN) ((BYTELEN + 1) & (~1))

/* convert between host and network(big endian) */
#define ZBYTE_HN_CVAL(buf,pos)          (ZBYTE_CVAL(buf, pos))
#define ZBYTE_HN_SCVAL(buf,pos,val)     (ZBYTE_SCVAL(buf, pos, val))

#define ZBYTE_HN_SVAL(buf,pos)          (ZBYTE_RSVAL(buf, pos))
#define ZBYTE_HN_SVALS(buf,pos)         (ZBYTE_RSVALS(buf, pos))
#define ZBYTE_HN_SSVAL(buf,pos, val)    (ZBYTE_RSSVAL(buf, pos, val))
#define ZBYTE_HN_SSVALS(buf,pos, val)   (ZBYTE_RSSVALS(buf, pos, val))
#define ZBYTE_HN_IVAL(buf,pos)          (ZBYTE_RIVAL(buf, pos))
#define ZBYTE_HN_IVALS(buf,pos)         (ZBYTE_RIVALS(buf, pos))
#define ZBYTE_HN_LVAL(buf,pos)          (ZBYTE_RLVAL(buf, pos))
#define ZBYTE_HN_LVALS(buf,pos)         (ZBYTE_RLVALS(buf, pos))
#define ZBYTE_HN_SIVAL(buf,pos, val)    (ZBYTE_RSIVAL(buf, pos, val))
#define ZBYTE_HN_SIVALS(buf,pos, val)   (ZBYTE_RSIVALS(buf, pos, val))
#define ZBYTE_HN_SLVAL(buf,pos, val)    (ZBYTE_RSIVAL(buf, pos, (int)(val>>32)), ZBYTE_RSIVAL(buf + 4, pos, (int)val))
#define ZBYTE_HN_SLVALS(buf,pos, val)   (ZBYTE_RSLVALS(buf, pos, val))
#define ZBYTE_HN_SREV(x)                ZBYTE_SREV(x)
#define ZBYTE_HN_IREV(x)                ZBYTE_IREV(x)
#define ZBYTE_HN_LREV(x)                ZBYTE_LREV(x)

#endif // __zce_bytes_h__
