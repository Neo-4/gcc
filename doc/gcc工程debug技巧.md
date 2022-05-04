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
