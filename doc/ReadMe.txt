
EBWin 3.05 have a feature called 'Wordbook'.
This feature works well only running EBWin.
But if you use menu 'Edit->Wordbook->Save Wordbook' and export your
wordbook, you will see too many code which can not be read by human :P
They are named 'ebcode' or 'Gaiji code'
The wordbook can not be imported into EBWin program,
And printing them is just wasting paper :(

The newest version EBWin4 do not support wordbook,
but it gives you some latest map files in your user-dir.


This toolkit is designed for convert you wordbook to readable UTF-8 text.

genmap.exe     Make conversion-list 'GAIJI.ZMP' for wbconv.exe use
wbconv.exe     Convert you wordbook!
GAIJI.ZMP      I made a conversion-list, which includes arranged ZHONG_RI.map
ReadMe.txt     Manual written by my poor English :P
ReadMe_chs.txt Manual written by Chinese Simplified

User Manual
=========================

wbconv.exe file1
::Current version is not stable
::Only convert file1 to out.txt
::You can drag your wordboot onto this file

genmap.exe [-a] map1 [map2 [...]]
           -a Add/Update mapfile(s) into existing conversion-list
::You should know some usage of command-line before use this file
::You can arrange your own map files befor use this file
::e.g. Importing DIC_BetA.map, means dictionary name is DIC_BetA, be careful

Copyright
=========================
Author: Sakura Asairo
Including 'getopt' and 'zlib' using LGPL
Licence of this project: GPL
