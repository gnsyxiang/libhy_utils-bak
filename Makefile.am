
ACLOCAL_AMFLAGS = -I m4

# 代码优化等级
# ----------------------------
# -O0		: 默认选项，目的: 减少编译时间;生成可调试代码(可打断)
# -O/O1 	: 做一些不显著增加编译时间的优化
# -O2 		: 显著提升编译时间，提升代码性能，做不包含`space-speed tradeoff`的所有优化
# -O3 		: 进一步优化，显著增加可执行文件大小。
# -Os 		: 优化性能同时不增加可执行文件大小。包含O2选项中不增加代码大小的优化项
# -Og 		: 优化性能同时不损害可调试性。包含O1选项中不损害可调试性的优化项
# -Ofast 	: 忽视严格的标准编译性。包含O3中所有选项及` -ffast-math`

# 警告信息
# ----------------------------
# -w        : 不输出警告
# -W(大写)  : 会显示警告，但是只显示编译器认为会出现错误的警告。
# -Wall    	: 输出所有的警告信息
# -Werror  	: 遇到警告时停止编译

# 调试信息
# ----------------------------
# -g       	: 增加调试信息
# -DNDEBUG  : "NDEBUG"是一个标准的ANSI宏，表示不进行调试编译。(该宏和assert有关)

# Define compiler options
# ----------------------------
# -pipe		: 在编译过程的不同阶段之间使用管道而非临时文件进行通信，可以加快编译速度。建议使用。

AM_CFLAGS_COM  = -pipe -W -Wall -Werror
AM_CFLAGS_COM += -I$(top_srcdir)/../inc
export AM_CFLAGS_COM

# 动态库
# ----------------------------
# -rdynamic 			: 用来通知链接器将所有符号添加到动态符号表中（目的是能够通过使用 dlopen 来实现向后跟踪）
# -Wl,--as-needed 		: 选项指示最终的可执行文件中只包含必要的链接库信息，
# -Wl,--no-as-needed 	: 选项指示在命令行中指定加载的所有库都记录到可执行文件头中，并最终由动态加载器去加载。
# -fPIC和-fpic
#  		相同点：都是为了在动态库中生成位置无关的代码。通过全局偏移表（GOT）访问所有常量地址。程序启动时动态加载程序解析GOT条目。
#  		不同点：如果链接的可执行文件的GOT大小超过计算机特定的最大大小，则会从链接器收到错误消息，指示-fpic不起作用;在这种情况下，
#  		        请使用-fPIC重新编译。GOT大小根据操作系统的不同而大小不一样，SPARC上为8k，
#  		        在AArch64上为28k，在m68k和RS / 6000上为32k。x86没有此限制。

LDFLAGS_COM  = -rdynamic -fPIC
LDFLAGS_COM += -version-info @current@:@revision@:@age@
export LDFLAGS_COM

SUBDIRS=inc src res sample
# SUBDIRS=doc test

