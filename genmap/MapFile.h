#pragma once

// Entry Format:
// char* title;
// int entry_length;
// { char* ebcode; char* utf8; }

// 

// �Ե����ļ����б���
class MapFile
{
    std::string _title; // �ʵ����ƣ���Ҫ��
    std::map<std::string, std::string> _map;
    int _len; // map �ĳ��ȣ������ļ��洢
    bool _gbk_support; // �����պ��ʵ�

    void _push(const char* Line);
    void _set_title(const wchar_t* Name);

public:
    MapFile();
    MapFile(const wchar_t* InFile);
    MapFile(const char* InBuff);

    // Dest: UTF-8 string ��ȡ���
    int exchange(std::string& Rslt, const std::string& Key) const;

    // ��ȡԪ��
    std::string& title();
    std::map<std::string, std::string>& list();
};
