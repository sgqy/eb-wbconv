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
    if (i == _mmf.end()) // 匹配地图名称（map类型的文件名）
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
