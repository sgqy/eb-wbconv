#pragma once

// �Ե����ļ����б���
class MapFile
{
    std::string _title; // �ʵ����ƣ���Ҫ��
    std::map<std::string, std::string> _map;
    int _len; // map �ĳ��ȣ������ļ��洢

    // ������
    void _push(const char* Line);
    // �����б���
    void _set_title(const wchar_t* Name);
    // ��ʼ������
    void mf_init();

//public:
    // ���¶��߾�Ϊ ENABLE_GBK_CONV ����ת��
    int _sys_gbk_enable; // ϵͳ�������
    int _book_gbk_enable; // �鼮�����Ƿ�֧�� GBK, ע���ϵͳ�����޹�

public:
    MapFile();

    // ���ļ�����, ��Ҫ wmain ��֧��
    MapFile(const wchar_t* InFile);

    // ��ȡ���
    // Input: "&hA001;"  Output: UTF-8 String
    int exchange(std::string& Rslt, const std::string& Key) const;

    // ��ȡ���Ա
    const std::string& title() const;
    const std::map<std::string, std::string>& list() const;

    // �����Դ洢�а���ʽ��ȡ�������, ������Ŀ������
    int Import(const char* Buf);

    // ���Ա������е�����, ���� Buf �ĳ���
    // WARNING: Allocating memory WITHOUT delete!!!
    int Export(char*& Buf) const;

    // �����Ʊ����ʽ:
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
