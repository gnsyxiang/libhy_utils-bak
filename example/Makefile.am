# ===============================================================
# 
# Release under GPL-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    21/03 2020 14:10
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        21/03 2020      create the file
# 
#     last modified: 21/03 2020 14:10
# ===============================================================

CFLAGS_COM = -I$(top_srcdir)/include \
			 -Wno-error=stringop-truncation

hal_ld_com = -L$(top_builddir)/src/.libs \
			 -lutils -lpthread

bin_PROGRAMS 	=

if COMPILE_WITH_LIBCJSON_UTILS
bin_PROGRAMS 	+= cjson
cjson_SOURCES	 = cjson.c
cjson_LDADD   	 = $(hal_ld_com)
cjson_CFLAGS  	 = $(AM_CFLAGS_COM) $(CFLAGS_COM)
endif

bin_PROGRAMS			+= hy_uart_test
hy_uart_test_SOURCES 	 = hy_uart_test.c
hy_uart_test_LDADD   	 = $(hal_ld_com)
hy_uart_test_CFLAGS  	 = $(AM_CFLAGS_COM) $(CFLAGS_COM)

bin_PROGRAMS			+= hy_fifo_test
hy_fifo_test_SOURCES 	 = hy_fifo_test.c
hy_fifo_test_LDADD   	 = $(hal_ld_com)
hy_fifo_test_CFLAGS  	 = $(AM_CFLAGS_COM) $(CFLAGS_COM)

bin_PROGRAMS			+= list 
list_SOURCES 	 	 	 = list.c
list_LDADD   	 	 	 = $(hal_ld_com)
list_CFLAGS  	 	 	 = $(AM_CFLAGS_COM) $(CFLAGS_COM)
