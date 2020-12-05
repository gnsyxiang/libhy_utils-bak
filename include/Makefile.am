# ===============================================================
# 
# Release under GPL-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    21/03 2020 14:09
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        21/03 2020      create the file
# 
#     last modified: 21/03 2020 14:09
# ===============================================================

HEADERS_COM = hy_fifo.h \
			  hy_file.h \
			  hy_list.h \
			  hy_log.h \
			  hy_md5sum.h \
			  hy_time.h \
			  hy_type.h \
			  hy_utils.h

if COMPILE_WITH_MCU_VERSION
else
HEADERS_COM += hy_uart.h \
			   hy_socket.h \
			   hy_pthread.h
endif

if COMPILE_WITH_LIBCJSON_UTILS
HEADERS_COM += hy_cjson.h hy_cjson_file.h
endif

noinst_HEADERS  =

halincludedir = $(includedir)/utils

halinclude_HEADERS = $(HEADERS_COM)

