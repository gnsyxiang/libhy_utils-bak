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

CFLAGS_COM  = -I$(top_srcdir)/include
CFLAGS_COM += -Wno-unused-parameter

hal_ld_com  = -L$(top_builddir)/src/.libs
hal_ld_com += -lutils

bin_PROGRAMS = demo

demo_SOURCES = demo.c
demo_LDADD   = $(hal_ld_com)
demo_CFLAGS  = $(AM_CFLAGS_COM) $(CFLAGS_COM)
