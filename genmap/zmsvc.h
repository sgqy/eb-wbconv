
#ifndef ZMSVC_H
#define ZMSVC_H

#ifdef _DEBUG
#pragma comment(lib, "./x86/ZlibStatDebug/zlibstat.lib")
#else
#pragma comment(lib, "./x86/ZlibStatRelease/zlibstat.lib")
#endif

#define ZLIB_WINAPI

#endif
