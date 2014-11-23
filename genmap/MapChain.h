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

// ����ļ�����
class MapChain
{
    // <title, map_file>
    std::map<std::string, MapFile> _mfp;
public:
    MapChain();

    // ���ļ�����, ��Ҫ wmain ��֧��
    MapChain(const wchar_t* InFile);

    // ��ȡ���
    // Input: "&hA001;", "DICNAME"  Output: UTF-8 String
    int exchange(std::string& Rslt, const std::string& Key, const std::string& Title) const;
    
    // ���Ա�������, ���� Buf �ĳ���
    // WARNING: Allocating memory WITHOUT delete!!!
    int Export(char*& Buf) const;

    // �����Դ洢�а���ʽ��ȡ�������, ������Ŀ������
    int Import(const char* Buf);

    // ��װ MapFile �ṹ
    MapChain& operator += (const MapFile&);
};
