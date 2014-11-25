
#ifndef SURROGATE_H_INCLUDED
#define SURROGATE_H_INCLUDED

//#ifdef __cplusplus
//extern "C" {
//#endif // __cplusplus


/** return length of dst */
int utf16_to_ucs2(wchar_t* dst, const int src);

/** return utf16 value */
int ucs2_to_utf16(const wchar_t* src);

//#ifdef __cplusplus
//}
//#endif // __cplusplus

#endif // SURROGATE_H_INCLUDED
