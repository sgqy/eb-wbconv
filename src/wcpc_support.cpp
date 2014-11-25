
// 此文件来自项目 wcpc.
// https://github.com/wiki908/wcpc
// 注意转换前后长度均指字节数，非字符数

#include "stdafx.h"
#include "conf.h"

#define SENTENCE_LEN 4096 // 每行最大长度 4 MiB // 后续版本将添加自定义缓存容量
#define MEMPROTECT mem_s

inline void reverse_utf16(wchar_t* ws, const int wsl)
{
    for (int i = 0; i < wsl; ++i) {
        ws[i] = (ws[i] << 8) | (ws[i] >> 8);
    }
}

void wcpc(const int scp, const void* src, const int srcl,
    const int tcp, void* tag, int& tagl,
    const char dft, bool& islost)
{
    wchar_t* wstmp = new wchar_t[MEMPROTECT(SENTENCE_LEN * 2)]; // 建立缓存用于生成 UTF-16 序列

    int wslen = 0; // wstmp 中 UTF-16 序列的长度, 按照 wchar_t 计算, 和 WinAPI 保持一致

    switch (scp) { // 代码页
    case 1200: // UTF-16LE
        memcpy_s(wstmp, MEMPROTECT(SENTENCE_LEN * 2), src, srcl + 1);
        wslen = (srcl + 1) / 2;
        break;
    case 1201: // UTF-16BE
        memcpy_s(wstmp, MEMPROTECT(SENTENCE_LEN * 2), src, srcl);
        wslen = srcl / 2;
        reverse_utf16(wstmp, wslen);
        break;
    default: // UTF-8 || ANSI
        wslen = MultiByteToWideChar(scp, 0, (LPCCH)src, srcl, NULL, 0);
        MultiByteToWideChar(scp, 0, (LPCCH)src, srcl, wstmp, wslen);
    }

    switch (tcp) {
    case 1200: // UTF-16LE
        memcpy_s(tag, MEMPROTECT(SENTENCE_LEN * 2), wstmp, tagl = wslen * 2);
        break;
    case 1201: // UTF-16BE
        reverse_utf16(wstmp, wslen);
        memcpy_s(tag, MEMPROTECT(SENTENCE_LEN * 2), wstmp, tagl = wslen * 2);
        break;
    default: // UTF-8 || ANSI
        int lost = 0;
        int* plost = (tcp == 65001) ? NULL : &lost; // UTF-8 的话有 BUG ...
        const char* pdft = (tcp == 65001) ? NULL : &dft;
        tagl = WideCharToMultiByte(tcp, 0, wstmp, wslen, NULL, NULL, pdft, plost);
        WideCharToMultiByte(tcp, 0, wstmp, wslen, (LPSTR)tag, tagl, pdft, plost);
        if (lost == TRUE) {
            islost = true;
        }
        else {
            islost = false;
        }
    }

    delete[] wstmp; // 清空缓存
}
