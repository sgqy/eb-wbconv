#pragma once

#pragma pack(1)

// Package of MapFile
struct mfp_hdr
{
    int hdr_length;
    int entry_count;
};

struct mfp_info
{
    int offset;
    int length;
};

#pragma pack()

// 多个文件保存
class MapChain
{
    // <title, map_file>
    std::map<std::string, MapFile> _mfp;
public:
    MapChain();

    // 从文件读入, 需要 wmain 的支持
    MapChain(const wchar_t* InFile);

    // 获取配对
    // Input: "&hA001;", "DICNAME"  Output: UTF-8 String
    int exchange(std::string& Rslt, const std::string& Key, const std::string& Title) const;
    
    // 线性保存数据, 返回 Buf 的长度
    // WARNING: Allocating memory WITHOUT delete!!!
    int Export(char*& Buf) const;

    // 从线性存储中按格式读取类的数据, 返回条目的数量
    int Import(const char* Buf);

    // 组装 MapFile 结构
    MapChain& operator += (const MapFile&);
};
