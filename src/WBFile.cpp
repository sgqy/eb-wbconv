
#include "stdafx.h"
#include <vector>
#include <string.h>

#include "conf.h"
#include "MapFile.h"
#include "MapChain.h"
#include "WBFile.h"


static void str_cnv_mf(const std::string& Dict, std::string& Conv, const MapChain& MapPack)
{
    std::string out;
    std::string key;
    std::string rslt;

    int left = 0;
    int right = 0;

    while (1)
    {
        right = Conv.find('&', left);
        out += Conv.substr(left, right - left);
        if (right == std::string::npos) // 未找到 '&'
        {
            break;
        }
        else // 找到 '&'
        {
            left = right;
            right = Conv.find(';', left);
            key = Conv.substr(left, (++right)-left); // 后面有个分号, 加进去
            //puts(key.c_str());

            MapPack.exchange(rslt, key, Dict);
            out += rslt;
        }
        left = right;
    }

    Conv = out;
}

void WBItem::convert(const MapChain& MapPack)
{
    str_cnv_mf(_dict, _title, MapPack);
    str_cnv_mf(_dict, _honbun, MapPack);
}

// 这是用字符分割字符串的实例
void WBItem::push(char* pLine)
{
    char* p = strrchr(pLine, '\n');
    if (p) *p = 0;
    p = strrchr(pLine, '\r');
    if (p) *p = 0;
    std::vector<std::string> Chain;
    int sp = '\t';
    std::string Src = pLine;

    int left = 0;
    int right = 0;

    int entry_count = 5; // vector 中必须有 5 个, 哪怕为空
    while (1)
    {
        right = Src.find(sp, left);
        Chain.push_back(Src.substr(left, right - left));
        --entry_count;
        left = right + 1;
        if (right == std::string::npos) break;
    }

    for (int i = 0; i < entry_count; ++i) // 如果单词表中有缺失项则补充空项目
    {
        Chain.push_back(std::string());
    }
    
    _index = strtol(Chain[0].c_str(), 0, 10);
    _dict = Chain[2];
    _title = Chain[3];
    _honbun = Chain[4];
}

void WBItem::pull(std::string& Dict, std::string& Title, std::string& Honbun) const
{
    Dict = _dict;
    Title = _title;
    Honbun = _honbun;
}

int WBItem::index() const
{
    return _index;
}

/////////////////////////////////////////////////////
WBFile::WBFile()
{

}

WBFile::WBFile(const wchar_t* InFile)
{
    scan(InFile);
}

void WBFile::convert(const MapChain& MapPack)
{
    auto it_end = _wbf.end();

    for (auto it = _wbf.begin(); it != it_end; ++it)
    {
        it->second.convert(MapPack);
    }
}

void WBFile::scan(const wchar_t* InFile)
{
    WBItem wbi;

    FILE* fp = 0;
    _wfopen_s(&fp, InFile, L"r"); // MSVC

    if (!fp) throw FILE_OPEN_ERR;

    char* temp = 0;
    temp = new char[3000];

    while (1)
    {
        fgets(temp, 3000, fp);
        if (feof(fp)) break;
        wbi.push(temp);
        _wbf[wbi.index()] = wbi;
    }

    delete[] temp;
    fclose(fp);
}

void WBFile::print(const wchar_t* OutFile)
{
    FILE* fp = 0;
    _wfopen_s(&fp, OutFile, L"w+"); // MSVC

    if (!fp) throw FILE_OPEN_ERR;
    auto it_end = _wbf.end();

    std::string Dict, Title, Honbun;
    for (auto it = _wbf.begin(); it != it_end; ++it)
    {
        it->second.pull(Dict, Title, Honbun);
        fprintf(fp, "%3d (%s)\t%s\n%s\n\n", it->first, Dict.c_str(), Title.c_str(), Honbun.c_str());
    }

    fclose(fp);
}
