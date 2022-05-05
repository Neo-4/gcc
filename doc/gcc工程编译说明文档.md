# 安装前的准备工作
## 1、下载源码
从git上，将源码下载到本地磁盘中。最好不要下载压缩包，直接通过git clone来下载。因为在windows下解压，会导致在linux系统由于部分文件的权限问题，没有办法完成编译。
## 2、安装linux环境
### 2.1、安装linux系统
首先，需要安装一个linux系统，用于编译gcc工程。windows10以上的系统，可以直接从windows商店里面，下载一个linux子系统，比如debian等。 
### 2.2、安装工具和库
然后在linux系统中安装编译gcc工程需要用到的工具以及库。  
#### 编译器
    1、gcc
    编译器需要支持ISO C++98，因此需要gcc 4.8及以上版本即可。
    查看当前gcc版本命令: gcc -v
    2、g++
    如果不安装g++，那么编译的时候会报错：
    error: C++ compiler missing or inoperational。
#### C标准库
    C标准库有glibc、libc、uclibc、klibc等很多类型，一般安装最常用的glibc库。
    查看当前glibc库的版本命令：ldd –version
#### Ada编译器
    因为gcc支持Ada语言，所以在编译gcc的时候，需要编译Ada语言编译器。
    因为Ada语言编译器是用Ada语言写的，因此使用GNAT编译器来编译Ada语言编译器。
    安装GNAT编译器命令：sudo apt-get install gnat
    查看gnat版本命令：gnat -v
#### GUN bash
    编译gcc工程，需要一个shell来执行一系列的脚本文件。
    常见的shell有bash、sh、ksh等，常见的linux系统一般都会带有bash。
#### GUN awk
    awk是一个文本分析工具。
    gcc的一部分源代码文件是在配置或者编译的时候通过awk来生成的。
#### GNU binutils
    binutils是一个操作二进制文件的二进制工具集。常见的像ld（连接器）、as(汇编器)。
    查看binutils版本命令：ld -v
#### gzip或者bzip2
    压缩和解压缩工具。需要gzip版本1.2.4及以上，bzip2版本1.0.2及以上。
    查看版本命令：
	gzip --version
	bzip2 –version
#### GUN make
    make工具，用来构建gcc工程的Makefile文件的。需要版本3.80及以上。
    查看版本：make -v
#### GUN tar
    打包程序。用来打包和解包一些文件。需要版本1.14及以上。
    查看版本：tar –version
#### Perl
    Perl是一种脚本语言。在编译gcc的时候，会使用到Perl语言的脚本。
    因此需要安装perl工具来执行这些Perl语言的脚本。需要版本5.6.1及以上。
    查看perl的版本命令：perl --version
#### GMP库
    GMP是一个任意精度的开源算术库，可用于符号整数，有理数，浮点数计算。
    可以直接安装GMP库，也可以下载GMP库的源代码，放到gcc的源码库目录下，
    将目录名命名为gmp，这样就会和gcc一起编译。需要版本4.3.2及以上。
    安装的GMP库的过程如下：
	1、查看libgmp安装包：apt-cache search libgmp
	2、安装GMP库：apt-get install libgmpxxx
	3、查看所有已经安装的库：ldconfig –p
    4、找到相应的libgmp库文件的目录后，通过查看目录名来确认库的版本号。
    比如：/usr/lib/x86_64-linux-gnu/
#### MPFR库
    MPRF和GMP一样，也是一个算术库。安装方法和GMP类似，可以安装MPRF库，
    也可以下载源码放到gcc源码目录mpfr中和gcc一起编译。需要版本2.4.2及以上。
    安装的MPFR库过程如下：
	1、查看libmpfr安装包：apt-cache search libmpfr
	2、安装MPFR库：apt-get install libmpfrxx
	3、查看所有已经安装的库：ldconfig-p
    4、找到对应的libmpfr库文件的目录后，通过查看目录名来确认库的版本号。
    比如：/usr/lib/x86_64-linux-gnu/
#### MPC库
    MPC库和MPFR、GMP一样，都是算术库。安装方法和GMP类似，可以安装MPC库，
    也可以下载源码放到gcc源码目录mpc中和gcc一起编译。需要版本0.8.1及以上。
    安装的MPC库过程如下：
	1、查看libmpc安装包：apt-cache search libmpc
	2、安装MPC库：apt-get install libmpcxx
	3、查看所有已经安装的库：ldconfig-p
    4、找到对应的limmpc库文件的目录后，通过查看目录名来确认库的版本号。
    比如：/usr/lib/x86_64-linux-gnu/
#### isl库
    和MPC库一样，也是算术库。只能安装isl库装。需要版本0.15及以上。
    安装isl库过程如下：
	1、查看libmpc安装包：apt-cache search libisl
	2、安装isl库：apt-get install libislxx
	3、查看所有已经安装的库：ldconfig-p
    4、找到对应的limmpc库文件的目录后，通过查看目录名来确认库的版本号。
    比如：/usr/lib/x86_64-linux-gnu/

# 编译gcc工程
## 1、配置gcc工程
    例子：
    /mnt/e/project/gcc/gcc-7/configure --prefix=/home/hsp/project/gcc-7/bin/ --disable-multilib --enable-languages=c,c++ --program-suffix=-7.5.0 --with-gmp-include=/usr/include/x86_64-linux-gnu/ --with-gmp-lib=/usr/lib/x86_64-linux-gnu/ --with-mpfr-include=/usr/include/ --with-mpfr-lib=/usr/lib/x86_64-linux-gnu/ --with-mpc-include=/usr/include/ --with-mpc-lib=/usr/lib/x86_64-linux-gnu/ --with-isl-include=/usr/include/ --with-isl-lib=/usr/lib/x86_64-linux-gnu/

### 1.1、创建目录
#### srcdir
    源代码存放的目录。
#### objdir
    编译源码的目录。用于执行make和make install的目录。
#### resultdir
    编译完成后，安装gcc可执行文件的目录。
    make install命令，会将objdir目录中的可执行程序gcc以及
    运行gcc依赖的库拷贝到该目录下。
### 1.2、配置gcc工程
    srcdir/configure 配置选项
在objdir目录中执行上述配置命令，完成对gcc工程的配置。
配置选项一般只需要配置以下选线：
#### --prefix = resultdir
	在执行make install的时候，会把编译生成的gcc文件拷贝到resultdir目录中。
#### --disable-multilib
	如果需要编译64位机器和32位机器的gcc，那么需要准备32位和64位的C标准库。
    如果只编译一种位数的机器，那么只需要一种C标准库就可以了。
    配置该选项表示选择只编译一种位数的机器。
#### --enable-languages = c, c++ 
	选择需要gcc支持哪几种语言编译。
    这里选择只支持编译c和c++语言。
#### --program-suffix = -7.1.0
	这里是为了生成的gcc变成gcc-7.1.0，这样容易和host系统的gcc区别开来。
#### 指定依赖的库的路径
    --with-gmp-include=/usr/include/x86_64-linux-gnu/
    --with-gmp-lib=/usr/lib/x86_64-linux-gnu/
    --with-mpfr-include=/usr/include/
    --with-mpfr-lib=/usr/lib/x86_64-linux-gnu/
    --with-mpc-include=/usr/include/
    --with-mpc-lib=/usr/lib/x86_64-linux-gnu/
    --with-isl-include=/usr/include/
    --with-isl-lib=/usr/lib/x86_64-linux-gnu/
因为host系统都已经安装了gmp、mpfr、mpc、isl库（需要安装xxx-dev才能用来作为开发使用），因此这里需要指定这些库的路径以及头文件的路径。  
通过命令“dkpg –L libgmp-dev” 可以获取这些库的头文件和安装目录。
# 2、编译gcc工程
    make –j8
    make install
在objdir目录中执行上述命令，完成编译和安装。  
make -j8命令负责编译，在objdir目录中生成的可执行文件和依赖的库。  
make install命令负责安装，会将objdir目录中编译生成的可执行文件和依赖的库拷贝到resultdir目录（配置选项--prefix=resultdir）中去。

# 3、运行gcc命令
    ln -s resultdir/bin/gcc-7.1.0 gcc
    ./gcc 程序名
注意，这里要使用./gcc不能使用gcc，因为gcc命令调用的是host系统自带的gcc命令，./gcc则是gcc工程编译出来的gcc-7.1.0程序。
