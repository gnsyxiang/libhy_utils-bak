dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    check_libhy_hal.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    08/05 2021 15:01
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        08/05 2021      create the file
dnl 
dnl     last modified: 08/05 2021 15:01
dnl ===============================================================

# CHECK_LIBHY_HAL()
# --------------------------------------------------------------
# check libhy_hal

AC_DEFUN([CHECK_LIBHY_HAL],
    [
        AC_ARG_WITH([libhy_hal],
                    [AS_HELP_STRING([--with-libhy_hal=@<:@yes|no@:>@], [use libhy_hal library @<:@default=yes@:>@])],
                    [],
                    [with_libhy_hal=yes])

        if test x"$with_libhy_hal" = xyes ; then
            have_libhy_hal=no

            if test x"$LIBHY_HAL_CFLAGS" != x -a x"$LIBHY_HAL_LIBS" != x; then
                CPPFLAGS="$LIBHY_HAL_CFLAGS $CPPFLAGS"
                LDFLAGS="$LIBHY_HAL_LIBS $LDFLAGS"
                LIBS="$LIBS -lhy_hal"

                # Can I compile and link it?
                AC_TRY_LINK([#include "hy_hal/hy_time.h"],
                            [HyTimeGetCurrentTime2Us();],
                            [libhy_hal_linked=yes],
                            [libhy_hal_linked=no])
                if test $libhy_hal_linked = no; then
                    AC_MSG_ERROR([assign LIBHY_HAL_CFLAGS or LIBHY_HAL_LIBS for libhy_hal])
                fi

                have_libhy_hal="yes (inc lib)"
            else
                PKG_CHECK_MODULES([LIBHY_HAL],
                                  [libhy_hal > 0.1.0],
                                  [have_libhy_hal="yes (pkg-config)"],
                                  [:])

                if test "$have_libhy_hal" != no; then
                    CPPFLAGS="$LIBHY_HAL_CFLAGS $CPPFLAGS"
                    LDFLAGS="$LIBHY_HAL_LIBS $LDFLAGS"
                else
                    # fall back to standard autoconf test
                    AC_CHECK_LIB([hy_hal],
                                 [HyTimeGetCurrentTime2Us],
                                 [
                                    AC_CHECK_HEADER([hy_hal/hy_time.h],
                                                    [have_libhy_hal="yes (autoconf test)"])
                                 ])
                fi

                if test "$have_libhy_hal" = no; then
                    AC_MSG_ERROR([external libhy_hal support requested but library not found])
                fi
            fi

            AC_DEFINE(HAVE_LIBHY_HAL, 1, [Have libhy_hal library])
            AM_CONDITIONAL([COMPILE_LIBHY_HAL], [test "$have_libhy_hal" != no])
        fi
    ])

