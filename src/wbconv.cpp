
#include "stdafx.h"
#include "conf.h"
#include "MapFile.h"
#include "MapChain.h"
#include "WBFile.h"

#include "zlibstat"

using namespace std;

#include "zmpname"

int wmain(int argc, wchar_t* argv[])
{
    try
    {
        if (argc < 2) return 0;

        wchar_t zmp_name[32768];
        get_zmp_name(zmp_name, argv[0]);

        MapChain mfp(zmp_name);
        WBFile wbf(argv[1]);
        wbf.convert(mfp);
        wbf.print(L"out.txt");
    }
    catch (...)
    {
        printf("[-]\n");
        return 1;
    }

    return 0;
}
