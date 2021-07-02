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
_sources_com    +=

_include_com    += -I$(top_srcdir)/include

_ldflags_com    += $(top_builddir)/src/lib@project_name@.la

if COMPILE_LIBHY_HAL
_include_com    += $(HY_HAL_INCS)
_ldflags_com    += $(HY_HAL_LIBS)
endif

if COMPILE_LIBLOG4CPLUS
_include_com    += $(LOG4CPLUS_INCS)
_ldflags_com    += $(LOG4CPLUS_LIBS)
endif

_cppflags_com   += \
                   $(CPPFLAGS_COM) \
                   $(_include_com)

_cflags_com     += $(CFLAGS_COM)

_cxxflags_com   += $(CXXFLAGS_COM)

_ldflags_com    += $(BIN_LDFLAGS_COM)

# -----------
# compile app
# -----------
bin_PROGRAMS            += test_hy_log
test_hy_log_SOURCES     = $(_sources_com) hy_log_test.c
test_hy_log_CPPFLAGS    = $(_cppflags_com)
test_hy_log_CFLAGS      = $(_cflags_com)
test_hy_log_CXXFLAGS    = $(_cxxflags_com)
test_hy_log_LDFLAGS     = $(_ldflags_com)
test_hy_log_LDADD       = $(_ldadd_com)

bin_PROGRAMS            += test_hy_sort
test_hy_sort_SOURCES    = $(_sources_com) hy_sort_test.c
test_hy_sort_CPPFLAGS   = $(_cppflags_com)
test_hy_sort_CFLAGS     = $(_cflags_com)
test_hy_sort_CXXFLAGS   = $(_cxxflags_com)
test_hy_sort_LDFLAGS    = $(_ldflags_com)
test_hy_sort_LDADD      = $(_ldadd_com)

