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

NO_INSTALL_HEADERS_COM 	=
noinst_HEADERS  		= $(NO_INSTALL_HEADERS_COM)

# hy_type.h文件与libhy_hal/include/hy_type.h同步
INSTALL_HEADERS_COM 	= hy_compile.h \
						  hy_error.h \
						  hy_math.h \
						  hy_log.h \
						  hy_list.h \
						  \
						  hy_type.h
install_includedir 		= $(includedir)/@project_name@
install_include_HEADERS = $(INSTALL_HEADERS_COM)
