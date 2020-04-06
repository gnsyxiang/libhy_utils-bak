# ===============================================================
# 
# Release under GPL-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    21/03 2020 13:57
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        21/03 2020      create the file
# 
#     last modified: 21/03 2020 13:57
# ===============================================================

lib_LTLIBRARIES = libutils.la

LDFLAGS_COM  = -version-info @current@:@revision@:@age@
LDFLAGS_COM += -rdynamic
LDFLAGS_COM += -export-symbols-regex 'Utils*'

CFLAGS_COM   = $(AM_CFLAGS_COM)
CFLAGS_COM  += -fPIC
CFLAGS_COM  += -Wno-unused-parameter
CFLAGS_COM  += -I$(top_srcdir)/include

SOURCES_COM  = utils_demo.c

if COMPILE_WITH_LIBCJSON_UTILS
SOURCES_COM	+= utils_cjson.c
endif

AM_CFLAGS 		     = $(CFLAGS_COM)
libutils_la_LDFLAGS  = $(LDFLAGS_COM)
libutils_la_SOURCES  = $(SOURCES_COM)
