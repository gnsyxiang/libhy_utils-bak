dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    enable_mylog.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    07/06 2021 08:33
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        07/06 2021      create the file
dnl 
dnl     last modified: 07/06 2021 08:33
dnl ===============================================================

# ENABLE_MYLOG()
# --------------------------------------------------------------
# enable my log

AC_DEFUN([ENABLE_MYLOG],
    [
        AC_ARG_ENABLE([mylog],
            [AS_HELP_STRING([--enable-mylog], [enable my log @<:@default=no@:>@])],
            [AC_DEFINE(HAVE_MYLOG, 1, [enable my log])]
            [enable_mylog=no])
        AM_CONDITIONAL([COMPILE_MYLOG], [test "x$enable_mylog" = "xyes"])
    ])

