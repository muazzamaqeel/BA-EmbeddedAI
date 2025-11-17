/**
  ******************************************************************************
  * @file    unicode.c
  * @brief   Minimal Unicode conversion for FatFs LFN (ANSI mode)
  * @note    Needed when _USE_LFN >= 1 and _LFN_UNICODE == 0
  ******************************************************************************
  */

#include "ff.h"

/*-----------------------------------------------------------------------*/
/* FF Convert: Local code to Unicode and vice versa                      */
/*-----------------------------------------------------------------------*/
WCHAR ff_convert (WCHAR chr, UINT dir)
{
    /* Simple 1:1 conversion for ASCII only */
    if (dir == 0) {
        if (chr < 0x80) return chr;  /* ASCII range */
        return '?';                  /* Non-ASCII replaced */
    } else {
        if (chr < 0x80) return chr;
        return '?';
    }
}

/*-----------------------------------------------------------------------*/
/* FF WToUpper: Uppercase conversion (for LFN comparisons)               */
/*-----------------------------------------------------------------------*/
WCHAR ff_wtoupper (WCHAR chr)
{
    if (chr >= 'a' && chr <= 'z')
        chr -= 0x20;
    return chr;
}
