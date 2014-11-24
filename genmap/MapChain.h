#pragma once

#pragma pack(1)

// Package of MapFile
// -------------------------
// |mfp_hdr|compressed_data| -> unzip_size DO NOT contain mfp_hdr
// -------------------------

struct mfp_hdr_t
{
    int hdr_length;
    int entry_count;
    int info_length;
    int zipped_size;
    int unzip_size;
};

struct mfp_info_t
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

    // д���ļ�
    void write(const wchar_t* OutFile);

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
