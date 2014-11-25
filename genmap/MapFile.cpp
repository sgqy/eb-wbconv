#include "stdafx.h"
#include "conf.h"

#include <ctype.h>
#include "MapFile.h"

void MapFile::mf_init()
{
    _len = 0;
    _book_gbk_enable = DISABLE_GBK_CONV;
    if (IsValidCodePage(936) == TRUE)
    {
        _sys_gbk_enable = ENABLE_GBK_CONV;
    }
    else
    {
        _sys_gbk_enable = DISABLE_GBK_CONV;
    }
}

MapFile::MapFile()
{
    mf_init();
}

MapFile::MapFile(const wchar_t* InFile)
{
    mf_init(); // Ҫ�������һ����ʼ������, ��Ҫ�����������캯��

    FILE* fp = 0;
    _wfopen_s(&fp, InFile, L"r"); // MSVC

    if (!fp) throw FILE_OPEN_ERR;

    char* temp = 0;
    temp = new char[3000];

    while (1)
    {
        fgets(temp, 3000, fp);
        if (feof(fp)) break;
        _push(temp);
    }

    delete [] temp;
    fclose(fp);
    _set_title(InFile);
}

void MapFile::_set_title(const wchar_t* Name)
{
    wchar_t ori[128] = { 0 };
    char dst[256] = { 0 };

    int  i = wcslen(Name);
    for (; i >= 0; --i)
    {
        if (Name[i] == L'\\') break;
    }
    i += 1;
    int j = 0;
    while (1)
    {
        if (Name[i] && Name[i] != L'.')
        {
            ori[j++] = Name[i++];
        }
        else
        {
            break;
        }
    }
    ori[j] = 0;

    int ccnt = 0;
    bool blst = false;
    wcpc(1200, ori, j * 2, 65001, dst, ccnt, '_', blst);
    //printf("%d", ccnt);
    dst[ccnt] = 0;

    _title = dst;
}

static void val_conv(char* s)
{
    int len = strlen(s);
    for (int i = 0; i < len; ++i)
    {
        switch (s[i])
        {
        case 'u':
        case ',':
        case 0xD:
        case 0xA:
            s[i] = ' ';
        }
    }

    // ������ʱ����
    wchar_t* temp = 0;
    temp = new wchar_t[40];
    int ti = 0;

    char* si = s;
    int utf16 = 0;
    while (1)
    {
        utf16 = strtol(si, &si, 16);
        if (!utf16) break; // ���� 0 ������, �������Ϣ, �� wchar_t temp[] Ϊ��, ���ս��Ϊ��
        ti += utf16_to_ucs2(temp + ti, utf16); // �� \u010000 �� \u10FFFF ���ж�Ӧ����
    }
    int taglen = 0;
    bool lost = false;
    wcpc(1200, temp, ti * 2, 65001, s, taglen, '_', lost);
    s[taglen] = 0;

    delete [] temp;
}


void MapFile::_push(const char* Line)
{
    // czgh #
    switch (Line[0])
    {
    case 'g':
        _book_gbk_enable = ENABLE_GBK_CONV; // ��Ӧ����֧�� GBK, ��ϵͳ�޹�!
    case 'h':
    case 'z':
    case 'c':
        goto Match;
    }
    return;
Match:
    char key[10] = { 0 }; // ��ߵ���ֵ

    char val[240] = { 0 }; // �ұߵĶ�Ӧ��ı���
    sscanf(Line, "%s %s", key, val);

    int key_len = strlen(key);
    for (int i = 1; i < key_len; ++i)
    {
        key[i] = toupper(key[i]);
    }

    val_conv(val); // �Ѷ�Ӧ�����������ת��Ϊ utf-8 ����

    std::string value;
    if (strlen(val) == 0) // �˽��: ԭ�ı��Ҳ��Ӧ��Ϊ��
    {
        // ��ִ�� _map ��¼�����;
    }
    else
    {
        value = val;
        _map[std::string(key)] = value;
    }
}

// Format of Key: "&hA001;"
int MapFile::exchange(std::string& Rslt, const std::string& Key) const
{
    int ret = FIND_SUCCESS;

    char key_cnv[10] = { 0 }; // ֻ�����ֱ�ɴ�д
    char key_raw[10] = { 0 }; // ���ڲ�ѯ�Ĳ���
    int key_len = Key.size();

    for (int i = 0; i < key_len; ++i)
    {
        if (i >= 2 && i < key_len - 1)
        {
            key_raw[i-1] = key_cnv[i] = toupper(Key[i]); // ���ֲ���, ע�ⲻ�� i-2
        }
        else if (i == 1)
        {
            key_raw[i-1] = key_cnv[i] = Key[i]; // chgz �ĸ���ͷ
        }
        else
        {
            key_cnv[i] = Key[i]; // '&' �� ';'
        }
    }

    std::string Raw = key_raw; // ��Ӧ����ĩβ�ķֺ�

    ////////////// ��ʼ�滻 //////////////

    // original: std::map<std::string, std::string>::const_iterator
    auto it = _map.find(Raw);
    if (it == _map.end()) // ���в����ڽ��
    {
        if (Raw[0] == 'g'
            && _book_gbk_enable == ENABLE_GBK_CONV
            && _sys_gbk_enable == ENABLE_GBK_CONV
            ) // ���� gbk ���յ�ֱ�ӽ���
        {
            unsigned char gbk[4] = { 0 }; // length: 2
            char cnv[12] = { 0 }; // length: 3~4

            char temp[3] = { 0 };
            temp[0] = Raw[1];
            temp[1] = Raw[2];
            gbk[0] = strtol(temp, 0, 16);
            temp[0] = Raw[3];
            temp[1] = Raw[4];
            gbk[1] = strtol(temp, 0, 16);

            int ccnt = 0;
            bool blst = false;
            wcpc(936, gbk, 2, 65001, cnv, ccnt, '_', blst);
            Rslt = cnv;
        }
        else
        {
            Rslt = key_cnv;
            ret = FIND_FALLBACK;
        }
    }
    else // ���д��ڽ��. ����: ����ѯ�����, ����ж�Ӧ��
    {
        Rslt = it->second;
    }

    return ret;
}

const std::map<std::string, std::string>& MapFile::list() const
{
    return _map;
}

const std::string& MapFile::title() const
{
    return _title;
}

// �����Դ洢�ж�ȡ MapFile �ṹ, ���ص�����Ŀ
int MapFile::Import(const char* Buf)
{
    // �������
    mf_hdr_t* hdr = (mf_hdr_t*)Buf;
    int buf_length = hdr->file_length;
    int entry_count = hdr->entry_count;
    _book_gbk_enable = hdr->is_book_gbk;

    // ��ʼ��ȡ�����Ϣ
    char* pEntry = (char*)(hdr + 1);
    char temp[32] = { 0 }; // ������ʱ�洢վ��

    char* pTemp = temp;
    while (*pTemp++ = *pEntry++); // ��ȡ�ǵ�����
    _title = temp;

    std::string Key;
    std::string Val;
    for (int i = 0; i < entry_count; ++i)
    {
        pTemp = temp;
        while (*pTemp++ = *pEntry++);
        Key = temp;

        pTemp = temp;
        while (*pTemp++ = *pEntry++);
        Val = temp;

        _map[Key] = Val;
    }

    return entry_count;
}

// ���Ա��� MapFile �ṹ�������, ���ػ�������С
int MapFile::Export(char* Buf) const
{
    int buf_length = LinearSize();
    int entry_count = _map.size();

    auto it_end = _map.end();

    // д������
    mf_hdr_t* hdr = (mf_hdr_t*)Buf;
    hdr->file_length = buf_length;
    hdr->entry_count = entry_count;
    hdr->is_book_gbk = _book_gbk_enable;
    ++hdr;

    // ��ʼ¼��
    char* pEntry = (char*)hdr;
    const char* pSour = 0;
    pSour = _title.c_str();
    while (*pEntry++ = *pSour++);

    for (auto it = _map.begin(); it != it_end; ++it)
    {
        pSour = it->first.c_str();
        while (*pEntry++ = *pSour++);
        pSour = it->second.c_str();
        while (*pEntry++ = *pSour++);
    }

    return buf_length;
}

int MapFile::LinearSize() const
{
    int buf_length =
          sizeof(mf_hdr_t)  // header_length
        + _title.size() + 1 // title_length + \0
        ;

    auto it_end = _map.end();

    // �������������
    for (auto it = _map.begin(); it != it_end; ++it)
    {
        buf_length += it->first.size() + 1;
        buf_length += it->second.size() + 1;
    }

    return buf_length;
}
