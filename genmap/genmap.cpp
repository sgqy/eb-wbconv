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
        /////////////////////////////////////////////
        ////_setmode(fileno(stdout), _O_U8TEXT);
        //ofstream of;
        //of.open("o.txt");
        //Start:
        //string str;
        //cin >> str;
        //auto pos = str.find(';');
        //if (pos == string::npos) throw DATA_INPUT_ERR;
        //string Raw = str.substr(1, pos-1);
        //cout << Raw << endl;

        //unsigned char gbk[4] = { 0 }; // length: 2
        //char cnv[12] = { 0 }; // length: 3~4

        //char temp[3] = { 0 };
        //temp[0] = Raw[1];
        //temp[1] = Raw[2];
        //gbk[0] = strtol(temp, 0, 16);
        //temp[0] = Raw[3];
        //temp[1] = Raw[4];
        //gbk[1] = strtol(temp, 0, 16);

        //int ccnt = 0;
        //bool blst = false;
        //wcpc(936, gbk, 2, 65001, cnv, ccnt, '_', blst);

        //cout << "gbk: " << gbk << endl;
        //of << "utf8 " << cnv << endl;
        //goto Start;
        /////////////////////////////////////////////
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

