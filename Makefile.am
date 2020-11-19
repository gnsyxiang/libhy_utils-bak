# ===============================================================
# 
# Release under GPL-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    21/03 2020 14:15
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        21/03 2020      create the file
# 
#     last modified: 21/03 2020 14:15
# ===============================================================

ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS_COM = $(ACLOCAL_AMFLAGS) \
				-pipe -W -Wall -Werror \
				-Wno-unused-parameter
export AM_CFLAGS_COM

dist_doc_DATA = README.md

SUBDIRS = include src example res

