## 1、将debug信息写入log文件
```
#include <stdio.h>
FILE *file;
file = fopen("hspdebug.txt", "a");
fprintf(file,"this is a debug info.\n");
fclose(file);
```
## 2、快速找到结构体定义的位置
在源码中，错误的使用结构体定义变量，通过编译器来帮忙找出结构体定义的位置。比如：  
1、想要找到结构体c_parser{}的定义。直接错误的使用该结构体：  
`union c_parser test;`  
那么编译器就会发出报错,从而根据报错信息，找到结构体的定义。  
`gcc/c/c-parser.c:162.16: note: "struct c_parser" was previously declared here`

## 3、通过命令参数控制debug信息输出
因为在编译gcc工程的时候，会调用新生成的gcc命令来编译一些库，所以会在编译过程中增加很多没有必要的debug信息，或者导致编译失败。
因此可以通过增加一个gcc命令参数，实现在编译gcc工程的时候，不输出debug信息。只有在需要输出debug信息的时候，才把该参数加上。
增加命令参数的方式参考文档《cc命令参数》和《gcc命令参数》