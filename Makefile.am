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

# ------------------
# defining variables
# ------------------
_no_error_com       =
_remove_unuse_func  =
_install_files_com  =
_dist_files_com     =

CPPFLAGS_COM        =
CFLAGS_COM          =
CXXFLAGS_COM        =
LIB_LDFLAGS_COM     =
BIN_LDFLAGS_COM     =

ACLOCAL_AMFLAGS     = -I m4
pkgconfigdir        = $(libdir)/pkgconfig

# ---------------------
# add necessary content
# ---------------------
_no_error_com       += \
                       -W -Wall -Werror \
                       -Wno-error=unused-but-set-variable \
                       -Wno-error=unused-variable \
                       -Wno-error=unused-result \
                       -Wno-unused-parameter

CPPFLAGS_COM        += \
                       -pipe \
                       $(_no_error_com) \
                       $(ACLOCAL_AMFLAGS)

LIB_LDFLAGS_COM     += -rdynamic

_remove_unuse_func  += \
                       -ffunction-sections \
                       -fdata-sections
CFLAGS_COM          += $(_remove_unuse_func)
CXXFLAGS_COM        += $(_remove_unuse_func)
BIN_LDFLAGS_COM     += -Wl,--gc-sections

if COMPILE_MCU_AT32F407VGT7
CPPFLAGS_COM        += $(AT32F407VGT7_CFLAGS)
endif


if COMPILE_SELECT_TARGET_OS_LINUX

if COMPILE_LIBLOG4CPLUS
CPPFLAGS_COM        += $(LOG4CPLUS_INCS)
endif

if COMPILE_ALSA_LIB
CPPFLAGS_COM        += $(ALSA_LIB_INCS)
endif

endif

export CPPFLAGS_COM
export CFLAGS_COM
export CXXFLAGS_COM
export LIB_LDFLAGS_COM
export BIN_LDFLAGS_COM

# ------------------
# make install files
# ------------------
_install_files_com  += \
                       LICENSE \
                       README.md
dist_doc_DATA       = $(_install_files_com)
pkgconfig_DATA      = @project_name@.pc

# ---------------
# make dist files
# ---------------
_dist_files_com     += \
                       autogen.sh \
                       build.sh \
                       @project_name@.pc.in
EXTRA_DIST          = $(_dist_files_com)

if COMPILE_LIBLOG4CPLUS
install-exec-hook:
	sed -i 's/-lhy_utils/-lhy_utils -llog4cplus/g' \
		$(top_builddir)/@project_name@.pc
endif

