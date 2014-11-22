#include "stdafx.h"
#include "conf.h"

#include "MapFile.h"

MapFile::MapFile()
{
    _len = 0;
    _gbk_support = false;
}

MapFile::MapFile(const wchar_t* InFile)
{
    FILE* fp = 0;
    _wfopen_s(&fp, InFile, L"r");

    if (!fp) throw FILE_OPEN_ERR;

    char* temp = 0;
    temp = new char[3000];

    while (1)
    {
        fgets(temp, 3000, fp);
        if (feof(fp)) break;
        _push(temp);
    }

    delete temp;
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
    wcpc(1200, ori, j*2, 65001, dst, ccnt, '_', blst);
    //printf("%d", ccnt);
    dst[ccnt] = 0;

    _title = dst;
}

static void raw_conv(char* s)
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

    // 建立临时区域
    wchar_t* temp = 0;
    temp = new wchar_t[40];
    int ti = 0;

    char* si = s;
    int utf16 = 0;
    while (1)
    {
        utf16 = strtol(si, &si, 16);
        if (!utf16) break;
        ti += utf16_to_ucs2(temp + ti, utf16); // 对 \u010000 到 \u10FFFF 进行对应处理
    }
    int taglen = 0;
    bool lost = false;
    wcpc(1200, temp, ti*2, 65001, s, taglen, '_', lost);
    s[taglen] = 0;

    delete temp;
}

void MapFile::_push(const char* Line)
{
    // czgh #
    switch (Line[0])
    {
    case 'g':
        _gbk_support = true;
    case 'h':
    case 'z':
    case 'c':
        goto Match;
    }
    return;
Match:
    char key[10]; // 左边的数值
    char raw[64]; // 右边的对应项
    sscanf(Line, "%s %s", key, raw);
    raw_conv(raw); // 把对应项包含的数字转换为 utf-8 编码

    std::string value;
    if (strlen(raw) == 0)
    {
        strcpy(raw, key);
        value = std::string("\x01")
            //+ std::string(raw) + std::string(";")
            ;
    }
    else
    {
        value = raw;
    }

    _map[std::string(key)] = value;
}

// Format of Key: "&hA001;"
int MapFile::exchange(std::string& Rslt, const std::string& Key) const
{
    int ret = FIND_SUCCESS;
    
    auto pos = Key.find(';');
    if (pos == std::string::npos) throw DATA_INPUT_ERR;
    std::string Raw = Key.substr(1, pos-1); // 不应包含末尾的分号

    // original: std::map<std::string, std::string>::const_iterator
    auto it = _map.find(Raw);
    if (it == _map.end()) // 表中不存在结果
    {
        if (Raw[0] == 'g' && _gbk_support)
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
            Rslt = Key;
            ret = FIND_FALLBACK;
        }
    }
    else // 表中存在结果
    {
        Rslt = it->second;
        if (Rslt[0] == '\x01')
        {
            Rslt = Key;
            ret = FIND_FALLBACK;
        }
    }

    return ret;
}

std::map<std::string, std::string>& MapFile::list() 
{
    return _map;
}

std::string& MapFile::title()
{
    return _title;
}
