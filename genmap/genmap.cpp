// genmap.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "conf.h"
//#include "MapChain.h"
#include "MapFile.h"
#include <fstream>
#include <sstream>
#include <iostream>

//#include <fcntl.h>
//#include <io.h>

#include "zlibstat"

using namespace std;

int wmain(int argc, wchar_t* argv[])
{
    try
    {
        if (argc != 2) return -1;
        
        ofstream of;
        MapFile mf(argv[1]);
        
        of.open("test_rslt.txt");
        
        char Key[50];
        string Rslt;
        while (1)
        {
            cin >> Key;
            for (int i = 2; i < strlen(Key); ++i)
            {
                Key[i] = toupper(Key[i]);
            }
            mf.exchange(Rslt, Key);
            of << Rslt << endl;
        }
        
        of.close();
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
