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

HEADERS_COM 	 = hy_log.h \
				   hy_fifo.h

if COMPILE_WITH_LIBCJSON_UTILS
HEADERS_COM 	+= utils_cjson.h utils_cjson_file.h
endif

noinst_HEADERS  =

halincludedir = $(includedir)/utils

halinclude_HEADERS = $(HEADERS_COM)

