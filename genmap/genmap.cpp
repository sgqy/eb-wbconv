// genmap.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "conf.h"
#include "MapFile.h"
#include "MapChain.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "zlibstat"

using namespace std;

int wmain(int argc, wchar_t* argv[])
{
    try
    {
        if (argc < 2) throw 1;

        MapChain mfp;
        for (int i = 1; i < argc; ++i)
        {
            mfp += MapFile(argv[i]);
        }

        mfp.write(L"o.txt");
    }
    catch (int& e)
    {
        printf("[-]Error: %d\n", e);
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
