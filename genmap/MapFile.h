#pragma once

// 对单个文件进行保存
class MapFile
{
    std::string _title; // 词典名称（重要）
    std::map<std::string, std::string> _map;
    int _len; // map 的长度，用于文件存储

    // 处理单行
    void _push(const char* Line);
    // 设置列表名
    void _set_title(const wchar_t* Name);
    // 初始化变量
    void mf_init();

//public:
    // 以下二者均为 ENABLE_GBK_CONV 方可转换
    int _sys_gbk_enable; // 系统环境检测
    int _book_gbk_enable; // 书籍本身是否支持 GBK, 注意和系统环境无关

public:
    MapFile();

    // 从文件读入, 需要 wmain 的支持
    MapFile(const wchar_t* InFile);

    // 获取配对
    // Input: "&hA001;"  Output: UTF-8 String
    int exchange(std::string& Rslt, const std::string& Key) const;

    // 获取类成员
    const std::string& title() const;
    const std::map<std::string, std::string>& list() const;

    // 从线性存储中按格式读取类的数据, 返回条目的数量
    int Import(const char* Buf);

    // 线性保存类中的数据, 返回 Buf 的长度
    // WARNING: Allocating memory WITHOUT delete!!!
    int Export(char*& Buf) const;

    // 二进制保存格式:
    //struct fmt
    //{
    //    int buf_length;
    //    int entry_count;
    //    int book_gbk;
    //    char* title;
    //    struct data
    //    {
    //        char* key;
    //        char* data;
    //    };
    //};
};
