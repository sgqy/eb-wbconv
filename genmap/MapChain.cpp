#include "stdafx.h"
#include "conf.h"

#include "MapFile.h"
#include "MapChain.h"


MapChain::MapChain()
{
}

int MapChain::get(std::string& Rslt, const std::string& Title, const std::string& Key)
{
    std::map<std::string, MapFile>::iterator i = _mmf.find(Title);
    if (i == _mmf.end()) // ƥ���ͼ���ƣ�map���͵��ļ�����
    {
        Rslt = Key;
        return FIND_FALLBACK;
    }
    else
    {
        return (i->second).exchange(Rslt, Key);
    }
}

MapChain::~MapChain()
{
}
