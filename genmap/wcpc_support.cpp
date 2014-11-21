
// ���ļ�������Ŀ wcpc.
// https://github.com/wiki908/wcpc
// ע��ת��ǰ�󳤶Ⱦ�ָ�ֽ��������ַ���

#include "stdafx.h"
#include "conf.h"

#define SENTENCE_LEN 4096 // ÿ����󳤶� 4 MiB // �����汾������Զ��建������
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
    wchar_t* wstmp = new wchar_t[MEMPROTECT(SENTENCE_LEN * 2)]; // ���������������� UTF-16 ����

    int wslen = 0; // wstmp �� UTF-16 ���еĳ���, ���� wchar_t ����, �� WinAPI ����һ��

    switch (scp) { // ����ҳ
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
        int* plost = (tcp == 65001) ? NULL : &lost; // UTF-8 �Ļ��� BUG ...
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

    delete[] wstmp; // ��ջ���
}
