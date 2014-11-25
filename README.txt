eb-wbconv
=========

Convert Wordbooks made by EBWin to readable UTF-8 text files

=========
更新历史:

wbconv: v0.8
[inf]没有做足够的测试, 似乎有不少潜在的问题, 版本定为 v0.8
[fix]实测后发现不少 Gaiji Map 有未定义项
[-->]放宽列表录入条件, 支持直接对应 UTF-8 串 (不要以 u 和 - 开头)

arrange project
由于两个项目有大量重复文件, 现进行归类整理
[mod]修改所有源文件的编码和行尾

genmap: v1.0
Generate Mapfile ver 1.0 完成
用于把 map 文件整合进 GAIJI.ZMP 文件中, 以加速 wbconv 的处理速度
程序中带有简易使用方法
详细使用说明书将在全部项目完成后编写

dev: mfp-done
[add]添加方法 MapChain::Import
[fix]潜在的内存泄漏问题 (重要)
[-->]把所有 new[] 对应的操作由 delete 改为 delete[]
[fix]MapFile 结构中的输入输出方法应用结构体 mf_hdr_t 后, 方法Import 中 pEntry 的数值错误, 导致 MapChain 中的 Export 方法和 Import 方法不对称
[-->]MapFile::Import 中语句 char* pEntry = (char*)(hdr); 改为 char* pEntry = (char*)(hdr + 1);
[-->]输出文件一致性已通过 CRC-32 校验

dev: mfp-export-done
[fix]调整了从文本文件读入时, Key 的数字部分转换为大写的代码位置. 统一了录入, 导入, 查询时大小写统一
[fix]没有单独的计算 MapFile 长度的操作, 导致 MapChain 的线性保存操作出现冗余
[-->]为 MapFile 添加 LinearSize 方法, 只计算保存为线性状态时的长度 (线程不安全)
[mem]在分离 MapFile::LinearSize 方法时，buf_length 的加数 _title.size() + 1 之前多了一个分号, 导致最后 0 级压缩后的数据与设想中不匹配
[mem]zlib 的 compress2 的第 3 个参数 sourceLen 永远是真实长度, 不要添加分配内存时使用的保护宏

dev: mf-fix & mfp-find
[add]添加了 MapChain 结构, 可包装 MapFile 结构, 并完成了对 MapChain 的外字兑换
[fix]在执行 MapFile 的 exchange 函数时, Key 的输入会因为大小写而失败
[-->]对输入的 Key 进行大小写转换, 并简单优化对 Key 的处理方法

dev: fix: gbk_support-buff_io
[fix]线性保存的数据中没有保存对照表对 GBK 信息的支持开关
[-->]把 MapFile 的 GBK 支持信息存入线性空间
[fix]不支持 GBK 代码页的系统会出现错误结果
[-->]判断系统对 GBK 的支持情况, 仅当系统和外字表同时支持时才执行转码, 否则返回原信息

dev: mf-ele-done
[add]添加了 MapFile 的线性存储功能, 用于文件导入导出 (核心)
[fix]进一步优化, 表中未定义项目将直接跳过, 不会存储在结构中

dev: mf-ele-find
[add]添加了 MapFile 结构的外字表替换功能 (核心)
[fix]把外字表中的未定义项目用 0x01 作为替换对象, 为之后的线性保存做准备

dev: map-from-io-2
[fix]重新规划源文件顺序, 代码内容没有太大变动
[fix]测试了 zlib 静态库的工作情况, 并对链接器代码进行了适当修改

dev: map-from-io
[add]首先尝试建立一种叫做 MapFile 的结构, 并让它能够从文件读取相应的外字表
