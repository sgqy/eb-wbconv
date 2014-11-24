#include "stdafx.h"
#include "conf.h"

#include "MapFile.h"
#include "MapChain.h"


MapChain::MapChain()
{

}

MapChain::MapChain(const wchar_t* InFile)
{
    FILE* fp = 0;
    _wfopen_s(&fp, InFile, L"rb"); // MSVC

    if (fp == 0) throw FILE_OPEN_ERR;
    fseek(fp, 0, SEEK_END);

    int read_siz = ftell(fp);
    char* buf = new char[mem_s(read_siz)];

    fseek(fp, 0, SEEK_SET);

    fread(buf, sizeof(char), read_siz, fp);
    fclose(fp);

    Import(buf);

    delete buf;
}

void MapChain::write(const wchar_t* OutFile)
{
    FILE* fp = 0;
    _wfopen_s(&fp, OutFile, L"wb+"); // MSVC

    if (fp == 0) throw FILE_OPEN_ERR;
    char* buf = 0;
    int size = Export(buf);

    fwrite(buf, sizeof(char), size, fp);

    fclose(fp);
    delete buf;
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
    // 搜集初始文件头信息
    mfp_hdr_t hdr = { 0 };
    hdr.hdr_length = sizeof(mfp_hdr_t);
    hdr.entry_count = _mfp.size();

    // 建立文件列表 (包含于被压缩部分)
    mfp_info_t* info_list = new mfp_info_t[hdr.entry_count];
    hdr.info_length = hdr.entry_count * sizeof(mfp_info_t);

    auto it_end = _mfp.end();
    auto it = _mfp.begin();
    int i = 0;

    // 写入列表数据, 计算未压缩长度
    int offset = 0;
    for (/* i = 0, it = beg */; it != it_end && i < hdr.entry_count; ++it, ++i)
    {
        info_list[i].offset = offset;
        offset += (info_list[i].length = it->second.LinearSize());
    }

    hdr.unzip_size = hdr.info_length + offset;

    // 开始录入
    char* Raw = new char[mem_s(hdr.unzip_size)];
    memcpy(Raw, info_list, hdr.info_length);
    delete info_list;

    char* pRaw = Raw + hdr.info_length;
    for (it = _mfp.begin(), i = 0; it != it_end && i < hdr.entry_count; ++it, ++i)
    {
        pRaw += it->second.Export(pRaw);
    }

    // 压缩
    char* Zipped = new char[mem_s(hdr.unzip_size)];
    int zipped_size = mem_s(hdr.unzip_size);
    if (compress2((Bytef*)Zipped, (uLongf*)&zipped_size,
        (Bytef*)Raw, hdr.unzip_size, 9) != Z_OK)
        // 第 3 个参数 sourceLen 永远是真实长度, 不要添加保护宏
    {
        throw ZIP_ERROR;
    }
    hdr.zipped_size = zipped_size;
    delete Raw;

    // 输出
    delete Buf;
    Buf = new char[mem_s(hdr.hdr_length + hdr.zipped_size)];
    memcpy(Buf, &hdr, hdr.hdr_length);
    memcpy(Buf + hdr.hdr_length, Zipped, zipped_size);
    delete Zipped;

    return (hdr.hdr_length + hdr.zipped_size);
}

int MapChain::Import(const char* Buf)
{
    return 0;
}
