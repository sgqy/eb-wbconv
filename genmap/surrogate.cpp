
#include "stdafx.h"
#include "surrogate.h"

int utf16_to_ucs2(wchar_t* dst, const int src)
{
    if(src >= 0x010000 && src <= 0x10FFFF)
    {
        dst[0] = (src - 0x10000) / 0x400 + 0xD800;
        dst[1] = (src - 0x10000) % 0x400 + 0xDC00;
        return 2;
    }
    else if(src >= 0 && src <= 0xFFFF)
    {
        dst[0] = src;
        dst[1] = 0;
        return 1;
    }
    else
    {
        throw UCS2_INPUT_ERR;
    }
}

int ucs2_to_utf16(const wchar_t* src)
{
    if(src[0] >= 0xD800 && src[0] <= 0xDBFF && src[1] >= 0xDC00 && src[1] <= 0xDFFF)
    {
        return ( (src[0] - 0xD800) * 0x400 + (src[1] - 0xDC00) + 0x10000 );
    }
    else
    {
        throw UCS2_INPUT_ERR;
    }
}
