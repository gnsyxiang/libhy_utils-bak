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

# ------------------
# defining variables
# ------------------
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
_include_com    += -I$(top_srcdir)/include

_sources_com    +=

_cppflags_com   += \
                   $(CPPFLAGS_COM) \
                   $(_include_com)

_cflags_com     += $(CFLAGS_COM)

_cxxflags_com   += $(CXXFLAGS_COM)

_ldflags_com    += $(BIN_LDFLAGS_COM)

_ldadd_com      += $(top_builddir)/src/lib@project_name@.la

if COMPILE_SELECT_TARGET_OS_LINUX
_ldadd_com      += -ldl
endif

# -----------
# compile app
# -----------
bin_PROGRAMS            += hy_log_test
hy_log_test_SOURCES     = $(_sources_com) hy_log_test.c
hy_log_test_CPPFLAGS    = $(_cppflags_com)
hy_log_test_CFLAGS      = $(_cflags_com)
hy_log_test_CXXFLAGS    = $(_cxxflags_com)
hy_log_test_LDFLAGS     = $(_ldflags_com)
hy_log_test_LDADD       = $(_ldadd_com)

bin_PROGRAMS            += hy_sort_test
hy_sort_test_SOURCES    = $(_sources_com) hy_sort_test.c
hy_sort_test_CPPFLAGS   = $(_cppflags_com)
hy_sort_test_CFLAGS     = $(_cflags_com)
hy_sort_test_CXXFLAGS   = $(_cxxflags_com)
hy_sort_test_LDFLAGS    = $(_ldflags_com)
hy_sort_test_LDADD      = $(_ldadd_com)

if COMPILE_SELECT_TARGET_OS_LINUX
# bin_PROGRAMS                            += hy_timerwheel_rotation_test
# hy_timerwheel_rotation_test_SOURCES     = $(_sources_com) hy_timerwheel_rotation_test.c
# hy_timerwheel_rotation_test_CPPFLAGS    = $(_cppflags_com)
# hy_timerwheel_rotation_test_CFLAGS      = $(_cflags_com)
# hy_timerwheel_rotation_test_CXXFLAGS    = $(_cxxflags_com)
# hy_timerwheel_rotation_test_LDFLAGS     = $(_ldflags_com)
# hy_timerwheel_rotation_test_LDADD       = $(_ldadd_com)
endif

