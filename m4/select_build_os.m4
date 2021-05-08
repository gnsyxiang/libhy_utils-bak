dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    select_build_os.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    08/05 2021 21:22
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        08/05 2021      create the file
dnl 
dnl     last modified: 08/05 2021 21:22
dnl ===============================================================

# SELECT_BUILD_OS()
# --------------------------------------------------------------
# select build system os

AC_DEFUN([SELECT_BUILD_OS],
    [
        AC_ARG_WITH([build_system_os],
            [AS_HELP_STRING([--with-build_system_os=@<:@linux|mac|window@:>@], [select system os about @<:@linux|mac|window@:>@ @<:@default=linux@:>@])],
            [],
            [with_build_system_os=linux])

        case "$with_build_system_os" in
            linux)  AC_DEFINE(HAVE_SELECT_BUILD_SYSTEM_OS_LINUX, 1, [select linux build system]) ;;
            mac)    AC_DEFINE(HAVE_SELECT_BUILD_SYSTEM_OS_MAC, 1, [select mac build system]) ;;
            window) AC_DEFINE(HAVE_SELECT_BUILD_SYSTEM_OS_WINDOW, 1, [select window build system]) ;;
            *)      AC_MSG_ERROR([bad value ${with_build_system_os} for --with-build_system_os=@<:@linux|mac|window@:>@]) ;;
        esac

        AM_CONDITIONAL([COMPILE_SELECT_BUILD_SYSTEM_OS_LINUX], [test "x$with_build_system_os" = "xlinux"])
        AM_CONDITIONAL([COMPILE_SELECT_BUILD_SYSTEM_OS_MAC], [test "x$with_build_system_os" = "xmac"])
        AM_CONDITIONAL([COMPILE_SELECT_BUILD_SYSTEM_OS_WINDOW], [test "x$with_build_system_os" = "xwindow"])
    ])

