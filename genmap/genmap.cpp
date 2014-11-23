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
        if (argc != 2) throw 1;

        MapChain mfp;
        mfp += MapFile(argv[1]);

        string A;
        string B;
        ofstream of;
        of.open("out.txt", ios::trunc);

        while (1)
        {
            cin >> A;
            mfp.exchange(B, A, string("InputA"));
            of << B << endl;
        }
    }
    catch (int& e)
    {
        printf("[-]Error: %d\n", e);
        return e;
    }
    catch (...)
    {
        printf("[-]unknown\n");
        return -3;
    }

	return 0;
}
