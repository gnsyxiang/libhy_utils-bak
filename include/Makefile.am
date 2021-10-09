# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    05/03 2021 10:35
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        05/03 2021      create the file
# 
#     last modified: 05/03 2021 10:35
# ===============================================================

# ------------------
# defining variables
# ------------------
_no_install_headers_com =
_install_headers_com    =

install_includedir      = $(includedir)/@project_name@

# ---------------------
# add necessary content
# ---------------------
_no_install_headers_com +=

_install_headers_com    += \
                           hy_assert.h \
                           hy_bit.h \
                           hy_compile.h \
                           hy_error.h \
                           hy_fifo.h \
                           hy_list.h \
                           hy_log.h \
                           hy_math.h \
                           hy_md5sum.h \
                           hy_mem.h \
                           hy_module.h \
                           hy_sort.h \
                           hy_string.h \
                           hy_type.h \
                           hy_utils.h

if COMPILE_SELECT_TARGET_OS_LINUX
_install_headers_com    += \
                           hy_net_wired.h \
                           hy_timer.h \
                           hy_dlopen.h \
                           hy_thread.h
endif

# ------------------
# make install files
# ------------------
noinst_HEADERS          = $(_no_install_headers_com)
install_include_HEADERS = $(_install_headers_com)

