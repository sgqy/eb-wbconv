// genmap.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "conf.h"
#include "MapFile.h"
#include "MapChain.h"

#include "zlibstat"

using namespace std;

void get_zmp_name(wchar_t* zmp, const wchar_t* arg0)
{
    wcscpy(zmp, arg0);
    wchar_t* find = wcsrchr(zmp, L'\\');
    if(find) wcscpy(find, L"\\GAIJI.ZMP");
    else wcscpy(zmp, L".\\GAIJI.ZMP");
}

enum mode_e
{
    Null,
    Add,
};

void help()
{
    printf(
        "usage: genmap [-a] map1 [map2 [...]]\n"
        "       -a  Add/Update mapfile(s) into existing mapfile package\n"
        "       output: GAIJI.ZMP\n"
        );
    throw 0;
}

int wmain(int argc, wchar_t* argv[])
{
    try
    {
        // �������
        if (argc < 2) help();

        int opt = 0;
        opterr = 0;
        mode_e mode = Null;

        while (~(opt = getopt_w(argc, argv, L"a")))
        {
            switch (opt)
            {
            case 'a':
                mode = Add;
                break;
            default:
                help();
            }
        }

        wchar_t zmp_name[32768];
        get_zmp_name(zmp_name, argv[0]);

        argc -= optind;
        argv += optind;

        // ��ʼ��� map �ļ�
        MapChain mfp;
        try
        {
            if (mode == Add)
                mfp.read(zmp_name);
        }
        catch (int& e)
        {
            if (e == FILE_OPEN_ERR)
                printf("[-]warning: creating new file\n");
            else
                throw e;
        }
        
        // �������, mfp ���������/�滻������

        for (int i = 0; i < argc; ++i)
        {
            try
            {
                    mfp += MapFile(argv[i]);
            }
            catch (...)
            {
                wprintf(L"[-]file error: %s\n", argv[i]);
            }
        }

        mfp.write(zmp_name);
    }
    catch (int& e)
    {
        if(e) printf("[-]Error: %d\n", e);
        return e;
    }
    catch (std::bad_alloc e)
    {
        printf("[-]memory\n");
        return MEMORY_ERR;
    }
    catch (...)
    {
        printf("[-]unknown\n");
        return -3;
    }

	return 0;
}
