#include "stdafx.h"
#include "conf.h"

#include <ctype.h>
#include "MapFile.h"

void MapFile::mf_init()
{
    _len = 0;
    _gbk_support = false;
    if (IsValidCodePage(936) == TRUE)
    {
        _sys_gbk_enable = true;
    }
    else
    {
        _sys_gbk_enable = false;
    }
}

MapFile::MapFile()
{
    mf_init();
}

MapFile::MapFile(const wchar_t* InFile)
{
    mf_init(); // 要单独设计一个初始化函数, 不要调用其他构造函数

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

    // 建立临时区域
    wchar_t* temp = 0;
    temp = new wchar_t[40];
    int ti = 0;

    char* si = s;
    int utf16 = 0;
    while (1)
    {
        utf16 = strtol(si, &si, 16);
        if (!utf16) break; // 遇到 0 则跳出, 如果无信息, 则 wchar_t temp[] 为空, 最终结果为空
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

    int key_len = strlen(key);
    for (int i = 1; i < key_len; ++i)
    {
        key[i] = toupper(key[i]);
    }

    char val[64]; // 右边的对应项（文本）
    sscanf(Line, "%s %s", key, val);
    val_conv(val); // 把对应项包含的数字转换为 utf-8 编码

    std::string value;
    if (strlen(val) == 0) // 此结果: 原文本右侧对应项为空
    {
        // 不执行 _map 的录入操作;
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
    
    auto pos = Key.find(';');
    if (pos == std::string::npos) throw DATA_INPUT_ERR;
    std::string Raw = Key.substr(1, pos-1); // 不应包含末尾的分号

    // original: std::map<std::string, std::string>::const_iterator
    auto it = _map.find(Raw);
    if (it == _map.end()) // 表中不存在结果
    {
        if (Raw[0] == 'g' && _gbk_support == _sys_gbk_enable) // 包含 gbk 对照的直接解码
            // 后面要判断和 true 的相等, 不要直接用 if(... && _gbk_support)
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
    else // 表中存在结果. 更新: 若查询到结果, 则必有对应项
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

// 从线性存储中读取 MapFile 结构, 返回导入条目
int MapFile::Import(const char* Buf)
{
    // 获得两个数据
    int* pCount = (int*)Buf;
    int buf_length = *pCount++;
    int entry_count = *pCount++;

    // 开始获取相关信息
    char* pEntry = (char*)pCount;
    char temp[32] = { 0 }; // 建立临时存储站点

    char* pTemp = temp;
    while (*pTemp++ = *pEntry++); // 获取辞典名称
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

// 线性保存 MapFile 结构里的内容, 返回缓冲区大小
int MapFile::Export(char*& Buf) const
{
    int buf_length =
        + 4                 // buf_length
        + 4                 // entry_count
        + _title.size() + 1 // title_length + \0
        ;
    int entry_count = _map.size();

    auto it_end = _map.end();

    // 计算输出区长度
    for (auto it = _map.begin(); it != it_end; ++it)
    {
        buf_length += it->first.size() + 1;
        buf_length += it->second.size() + 1;
    }

    if (Buf) delete Buf;
    Buf = 0;

    Buf = new char[mem_s(buf_length)];
    
    // 写入两个数据
    int* pCount = (int*)Buf;
    *pCount++ = buf_length;
    *pCount++ = entry_count;

    // 开始录入
    char* pEntry = (char*)pCount;
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
