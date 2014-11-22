#pragma once

// Entry Format:
// char* title;
// int entry_length;
// { char* ebcode; char* utf8; }

// 

// 对单个文件进行保存
class MapFile
{
    std::string _title; // 词典名称（重要）
    std::map<std::string, std::string> _map;
    int _len; // map 的长度，用于文件存储
    bool _gbk_support; // 用于日汉词典

    void _push(const char* Line);
    void _set_title(const wchar_t* Name);

public:
    MapFile();
    MapFile(const wchar_t* InFile);

    // Dest: UTF-8 string 获取配对
    int exchange(std::string& Rslt, const std::string& Key) const;

    // 获取元素
    std::string& title();
    std::map<std::string, std::string>& list();

    // 二进制输入输出
    int Import(const char* Buf);
    int Export(char* Buf);

    // 二进制保存格式:
    //struct fmt
    //{
    //    int buf_length;
    //    int entry_count;
    //    char* title;
    //    struct data
    //    {
    //        char* key;
    //        char* data;
    //    };
    //};
};
