// genmap.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "MapChain.h"
#include "MapFile.h"
#include <fstream>

using namespace std;

int wmain(int argc, wchar_t* argv[])
{
    try
    {
        if (argc != 2) return 0;
        MapFile mf(argv[1]);
        map<string, string> _map = mf.get_map();

        map<string, string>::iterator it;

        ofstream of;
        of.open("genmap.txt", ios::trunc);
        of << mf.get_title() << endl;

        for (it = _map.begin(); it != _map.end(); ++it)
        {
            of << it->first << " " << it->second << endl;
        }
        of.close();
    }
    catch (int& e)
    {
        printf("[-]Error: %d", e);
    }

	return 0;
}

