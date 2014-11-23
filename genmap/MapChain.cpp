#include "stdafx.h"
#include "conf.h"

#include "MapFile.h"
#include "MapChain.h"


MapChain::MapChain()
{

}

MapChain::MapChain(const wchar_t* InFile)
{

}

MapChain& MapChain::operator += (const MapFile& mf)
{
    _mfp[mf.title()] = mf;
    return *this;
}

int MapChain::exchange(std::string& Rslt, const std::string& Key, const std::string& Title) const
{
    int ret = FIND_SUCCESS;

    auto book = _mfp.find(Title);
    if (book == _mfp.end())
    {
        char key_cnv[10] = { 0 }; // 只把数字变成大写
        int key_len = Key.size();

        for (int i = 0; i < key_len; ++i)
        {
            if (i >= 2 && i < key_len - 1)
            {
                key_cnv[i] = toupper(Key[i]); // 数字部分
            }
            else
            {
                key_cnv[i] = Key[i]; // '&h' 和 ';'
            }
        }

        Rslt = key_cnv;
        ret = FIND_FALLBACK;
    }
    else
    {
        ret = book->second.exchange(Rslt, Key);
    }

    return ret;
}

int MapChain::Export(char*& Buf) const
{
    return 0;
}
int MapChain::Import(const char* Buf)
{
    return 0;
}
