eb-wbconv
=========

Convert Wordbooks made by EBWin to readable UTF-8 text files

=========
更新历史:

dev: mf-fix & mfp-find
[add]添加了 MapChain 结构, 可包装 MapFile 结构, 并完成了对 MapChain 的外字兑换
[fix]问题: 在执行 MapFile 的 exchange 函数时, Key 的输入会因为大小写而失败
-> 修复: 对输入的 Key 进行大小写转换, 并简单优化对 Key 的处理方法

dev: fix: gbk_support-buff_io
[fix]问题: 线性保存的数据中没有保存对照表对 GBK 信息的支持开关
-> 修复: 把 MapFile 的 GBK 支持信息存入线性空间
[fix]问题: 不支持 GBK 代码页的系统会出现错误结果
-> 修复: 判断系统对 GBK 的支持情况, 仅当系统和外字表同时支持时才执行转码, 否则返回原信息

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
