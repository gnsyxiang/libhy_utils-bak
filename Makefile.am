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

include res/doxygen.am

SUBDIRS = \
          include \
          src \
          doc \
          res

if COMPILE_SELECT_TARGET_OS_LINUX
SUBDIRS += example
endif

# -----
# flags
# -----
ACLOCAL_AMFLAGS     = -I m4

_no_error_com       = -W -Wall -Werror \
                      -Wno-error=unused-but-set-variable \
                      -Wno-error=unused-variable \
                      -Wno-error=unused-result \
                      -Wno-unused-parameter

_remove_unuse_func  = -ffunction-sections \
                      -fdata-sections

CPPFLAGS_COM        = -pipe \
                      $(_no_error_com) \
                      $(ACLOCAL_AMFLAGS)

CFLAGS_COM          = $(_remove_unuse_func)

CXXFLAGS_COM        = \
                      $(_remove_unuse_func)

LIB_LDFLAGS_COM     = -rdynamic

BIN_LDFLAGS_COM     = \
                      -Wl,--gc-sections

export CPPFLAGS_COM
export CFLAGS_COM
export CXXFLAGS_COM
export LIB_LDFLAGS_COM
export BIN_LDFLAGS_COM

# ------------------
# make install files
# ------------------
_install_files_com  = LICENSE \
                      README.md
dist_doc_DATA       = $(_install_files_com)

pkgconfigdir        = $(libdir)/pkgconfig
pkgconfig_DATA      = @project_name@.pc

# ---------------
# make dist files
# ---------------
_dist_files_com     = \
                      autogen.sh \
                      build.sh \
                      @project_name@.pc.in
EXTRA_DIST          = $(_dist_files_com)

if COMPILE_LIBLOG4CPLUS
install-exec-hook:
	sed -i 's/-lhy_utils/-lhy_utils -llog4cplus/g' \
		$(top_builddir)/@project_name@.pc
endif

