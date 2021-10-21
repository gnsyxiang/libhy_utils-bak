# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    05/03 2021 10:38
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        05/03 2021      create the file
# 
#     last modified: 05/03 2021 10:38
# ===============================================================

# ------------------
# defining variables
# ------------------
_no_error_com   =
_include_com    =
_sources_com    =
_cppflags_com   =
_cflags_com     =
_cxxflags_com   =
_ldflags_com    =
_ldadd_com      =
bin_PROGRAMS    =

# ---------------------
# add necessary content
# ---------------------
_no_error_com   +=

_include_com    += -I$(top_srcdir)/include

_sources_com    +=

_cppflags_com   += \
                   $(CPPFLAGS_COM) \
                   $(_no_error_com) \
                   $(_include_com)

_cflags_com     += $(CFLAGS_COM)

_cxxflags_com   += $(CXXFLAGS_COM)

# 用户指定的库全部链接，不管有没有使用
# _ldflags_com    += -Wl,--no-as-needed

# 用户指定的库按需链接
_ldflags_com    += -Wl,--as-needed

# 部分库不管有没有使用都链接，其他库按照需要链接
# note: 指定参数后面的链接库一律按照前面的参数，直到修改相同功能的配置参数
# _ldflags_com    += -Wl,--no-as-needed -lpthread -Wl,--as-needed

# 不链接未使用函数，减小可执行文件大小
_ldflags_com    += -Wl,--gc-sections

_ldadd_com      += $(top_builddir)/src/lib@project_name@.la

if COMPILE_THREADS_POSIX
_ldadd_com      += $(PTHREAD)
endif

bin_PROGRAMS                += hy_log_test
hy_log_test_SOURCES         = $(_sources_com) hy_log_test.c
hy_log_test_CPPFLAGS        = $(_cppflags_com)
hy_log_test_CFLAGS          = $(_cflags_com)
hy_log_test_CXXFLAGS        = $(_cxxflags_com)
hy_log_test_LDFLAGS         = $(_ldflags_com)
hy_log_test_LDADD           = $(_ldadd_com)

bin_PROGRAMS                += hy_fifo_test
hy_fifo_test_SOURCES        = $(_sources_com) hy_fifo_test.c
hy_fifo_test_CPPFLAGS       = $(_cppflags_com)
hy_fifo_test_CFLAGS         = $(_cflags_com)
hy_fifo_test_CXXFLAGS       = $(_cxxflags_com)
hy_fifo_test_LDFLAGS        = $(_ldflags_com)
hy_fifo_test_LDADD          = $(_ldadd_com)

bin_PROGRAMS                += hy_list_test
hy_list_test_SOURCES        = $(_sources_com) hy_list_test.c
hy_list_test_CPPFLAGS       = $(_cppflags_com)
hy_list_test_CFLAGS         = $(_cflags_com)
hy_list_test_CXXFLAGS       = $(_cxxflags_com)
hy_list_test_LDFLAGS        = $(_ldflags_com)
hy_list_test_LDADD          = $(_ldadd_com)

bin_PROGRAMS                += hy_md5sum_test
hy_md5sum_test_SOURCES      = $(_sources_com) hy_md5sum_test.c
hy_md5sum_test_CPPFLAGS     = $(_cppflags_com)
hy_md5sum_test_CFLAGS       = $(_cflags_com)
hy_md5sum_test_CXXFLAGS     = $(_cxxflags_com)
hy_md5sum_test_LDFLAGS      = $(_ldflags_com)
hy_md5sum_test_LDADD        = $(_ldadd_com)

bin_PROGRAMS                += hy_sort_test
hy_sort_test_SOURCES        = $(_sources_com) hy_sort_test.c
hy_sort_test_CPPFLAGS       = $(_cppflags_com)
hy_sort_test_CFLAGS         = $(_cflags_com)
hy_sort_test_CXXFLAGS       = $(_cxxflags_com)
hy_sort_test_LDFLAGS        = $(_ldflags_com)
hy_sort_test_LDADD          = $(_ldadd_com)

bin_PROGRAMS                += hy_utils_test
hy_utils_test_SOURCES       = $(_sources_com) hy_utils_test.c
hy_utils_test_CPPFLAGS      = $(_cppflags_com)
hy_utils_test_CFLAGS        = $(_cflags_com)
hy_utils_test_CXXFLAGS      = $(_cxxflags_com)
hy_utils_test_LDFLAGS       = $(_ldflags_com)
hy_utils_test_LDADD         = $(_ldadd_com)

bin_PROGRAMS                += hy_template_test
hy_template_test_SOURCES    = $(_sources_com) hy_template_test.c
hy_template_test_CPPFLAGS   = $(_cppflags_com)
hy_template_test_CFLAGS     = $(_cflags_com)
hy_template_test_CXXFLAGS   = $(_cxxflags_com)
hy_template_test_LDFLAGS    = $(_ldflags_com)
hy_template_test_LDADD      = $(_ldadd_com)

if COMPILE_SELECT_TARGET_OS_LINUX
bin_PROGRAMS                += hy_dir_test
hy_dir_test_SOURCES         = $(_sources_com) hy_dir_test.c
hy_dir_test_CPPFLAGS        = $(_cppflags_com)
hy_dir_test_CFLAGS          = $(_cflags_com)
hy_dir_test_CXXFLAGS        = $(_cxxflags_com)
hy_dir_test_LDFLAGS         = $(_ldflags_com)
hy_dir_test_LDADD           = $(_ldadd_com)

bin_PROGRAMS                += hy_signal_test
hy_signal_test_SOURCES      = $(_sources_com) hy_signal_test.c
hy_signal_test_CPPFLAGS     = $(_cppflags_com)
hy_signal_test_CFLAGS       = $(_cflags_com)
hy_signal_test_CXXFLAGS     = $(_cxxflags_com)
hy_signal_test_LDFLAGS      = $(_ldflags_com)
hy_signal_test_LDADD        = $(_ldadd_com)

bin_PROGRAMS                += hy_net_wired_test
hy_net_wired_test_SOURCES   = $(_sources_com) hy_net_wired_test.c
hy_net_wired_test_CPPFLAGS  = $(_cppflags_com)
hy_net_wired_test_CFLAGS    = $(_cflags_com)
hy_net_wired_test_CXXFLAGS  = $(_cxxflags_com)
hy_net_wired_test_LDFLAGS   = $(_ldflags_com)
hy_net_wired_test_LDADD     = $(_ldadd_com)

bin_PROGRAMS                += hy_thread_test
hy_thread_test_SOURCES      = $(_sources_com) hy_thread_test.c
hy_thread_test_CPPFLAGS     = $(_cppflags_com)
hy_thread_test_CFLAGS       = $(_cflags_com)
hy_thread_test_CXXFLAGS     = $(_cxxflags_com)
hy_thread_test_LDFLAGS      = $(_ldflags_com)
hy_thread_test_LDADD        = $(_ldadd_com)

bin_PROGRAMS                += hy_timer_test
hy_timer_test_SOURCES       = $(_sources_com) hy_timer_test.c
hy_timer_test_CPPFLAGS      = $(_cppflags_com)
hy_timer_test_CFLAGS        = $(_cflags_com)
hy_timer_test_CXXFLAGS      = $(_cxxflags_com)
hy_timer_test_LDFLAGS       = $(_ldflags_com)
hy_timer_test_LDADD         = $(_ldadd_com)
endif
