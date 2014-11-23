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
        //if (argc != 2) return -1;
        //ofstream of;

        //MapFile mf(argv[1]);
        //cout << "1: " << mf._book_gbk_enable << endl;
        //
        //auto mfc = mf.list();
        //auto m_end = mfc.end();

        ////cerr << "1\n";

        //of.open("_l.txt", ios::trunc);
        //for (auto m = mfc.begin(); m != m_end; ++m)
        //{
        //    of << m->first << " " << m->second << endl;
        //}
        //of.close();

        //char* p = 0;
        //int pl = mf.Export(p);

        //of.open("_m.txt", ios::binary | ios::trunc);
        //of.write(p, pl);
        //of.close();

        //MapFile mf2;
        //mf2.Import(p);
        //delete p;
        //cout << "2: " << mf2._book_gbk_enable << endl;
        //
        //mfc = mf2.list();
        //m_end = mfc.end();

        //of.open("_r.txt", ios::trunc);
        //for (auto m = mfc.begin(); m != m_end; ++m)
        //{
        //    of << m->first << " " << m->second << endl;
        //}
        //of.close();

        //system("pause");
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
