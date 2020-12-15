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
			 $(AM_CFLAGS_COM) 

hal_ld_com = -L$(top_builddir)/src/.libs \
			 -lhy_utils

if COMPILE_WITH_MCU_VERSION
else
hal_ld_com += -lpthread
endif

bin_PROGRAMS 	=

if COMPILE_WITH_LIBCJSON_UTILS
bin_PROGRAMS 	+= hy_cjson_test
hy_cjson_test_SOURCES	 = hy_cjson_test.c
hy_cjson_test_LDADD   	 = $(hal_ld_com)
hy_cjson_test_CFLAGS  	 = $(CFLAGS_COM)
endif

bin_PROGRAMS			+= hy_socket_test
hy_socket_test_SOURCES 	 = hy_socket_test.c
hy_socket_test_LDADD   	 = $(hal_ld_com)
hy_socket_test_CFLAGS  	 = $(CFLAGS_COM)

bin_PROGRAMS			+= hy_fifo_test
hy_fifo_test_SOURCES 	 = hy_fifo_test.c
hy_fifo_test_LDADD   	 = $(hal_ld_com)
hy_fifo_test_CFLAGS  	 = $(CFLAGS_COM)

bin_PROGRAMS			+= hy_list_test 
hy_list_test_SOURCES 	 = hy_list_test.c
hy_list_test_LDADD   	 = $(hal_ld_com)
hy_list_test_CFLAGS  	 = $(CFLAGS_COM)

bin_PROGRAMS			+= hy_md5sum_test 
hy_md5sum_test_SOURCES 	 = hy_md5sum_test.c
hy_md5sum_test_LDADD   	 = $(hal_ld_com)
hy_md5sum_test_CFLAGS  	 = $(CFLAGS_COM)

if COMPILE_WITH_MCU_VERSION
else
bin_PROGRAMS			+= hy_uart_test
hy_uart_test_SOURCES 	 = hy_uart_test.c
hy_uart_test_LDADD   	 = $(hal_ld_com)
hy_uart_test_CFLAGS  	 = $(CFLAGS_COM)
endif

