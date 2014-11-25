
EBWin 3.05 里有个功能是 wordbook
这货在软件里用还行，最起码能看完全
但是，如果用菜单中的 Edit->Wordbook->Save Wordbook
保存出来的东西会带有一大堆看不懂的玩意
也就是传说中的天书 ebcode
另一个官方称呼叫 Gaiji
这些看不懂的玩意既不能导入又不能打印（打印了也看不懂）

对了，最新版的 EBWin4 没有单词本功能，不知以后会不会添加，效果如何
但是 EBWin4 在安装后会于用户文件夹里给出最新的 map 文件，可以借用


你手上的工具集就是用于把单词本中的天书转换成可读内容的
包里有这样的文件：

genmap.exe     制作 wbconv.exe 要用到的对照表 GAIJI.ZMP
wbconv.exe     转换你的单词本
GAIJI.ZMP      笔者制作的代码对照表，对 ZHONG_RI.MAP 进行了一定的补充
ReadMe.txt     英文说明书
ReadMe_chs.txt 简体中文说明书

使用说明书
=======================

wbconv.exe file1
::现在的版本并不是很完整，只是单纯地把 file1 转换后保存为 out.txt
::使用可以很傻瓜化：把你的单词本拖放在 wbconv.exe 上即可

genmap.exe [-a] map1 [map2 [...]]
           -a 使用后面所有 map 文件替换/添加进 GAIJI.ZMP 中
::这个程序的使用需要懂得一定的命令行知识
::你可以在使用前整理一下自己搜集到的 map 文件
::map 文件的文件名就是辞典名，切记不要弄错

版权
=======================
Author: Sakura Asairo/夏侯飛燕
使用了基于 LGPL 的 getopt 和 zlib
本项目使用 GPL