dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    check_liblog4cplus.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    08/05 2021 10:29
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        08/05 2021      create the file
dnl 
dnl     last modified: 08/05 2021 10:29
dnl ===============================================================

# CHECK_LIBLOG4CPLUS()
# --------------------------------------------------------------
# check liblog4cplus

AC_DEFUN([CHECK_LIBLOG4CPLUS],
    [
        AC_LANG_PUSH([C++])
        AC_ARG_WITH([liblog4cplus],
                    [AS_HELP_STRING([--with-liblog4cplus=@<:@yes|no@:>@], [use liblog4cplus library @<:@default=yes@:>@])],
                    [],
                    [with_liblog4cplus=yes])

        if test x"$with_liblog4cplus" = xyes ; then
            have_liblog4cplus=no

            if test x"$LIBLOG4CPLUS_CFLAGS" != x -a x"$LIBLOG4CPLUS_LIBS" != x; then
                CPPFLAGS="$LIBLOG4CPLUS_CFLAGS $CPPFLAGS"
                LDFLAGS="$LIBLOG4CPLUS_LIBS $LDFLAGS"
                LIBS="$LIBS -llog4cplus"

                # Can I compile and link it?
                AC_TRY_LINK([#include "log4cplus/log4cplus.h"],
                            [log4cplus::Logger::getRoot();],
                            [liblog4cplus_linked=yes],
                            [liblog4cplus_linked=no])
                if test $liblog4cplus_linked = no; then
                    AC_MSG_ERROR([assign LIBLOG4CPLUS_CFLAGS or LIBLOG4CPLUS_LIBS for liblog4cplus])
                fi

                have_liblog4cplus="yes (inc lib)"
            else
                PKG_CHECK_MODULES([LIBLOG4CPLUS],
                                  [log4cplus > 0.1.0],
                                  [have_liblog4cplus="yes (pkg-config)"],
                                  [:])

                if test "$have_liblog4cplus" != no; then
                    CPPFLAGS="$LIBLOG4CPLUS_CFLAGS $CPPFLAGS"
                    LDFLAGS="$LIBLOG4CPLUS_LIBS $LDFLAGS"
                else
                    # fall back to standard autoconf test
                    AC_CHECK_LIB([log4cplus],
                                 [_ZN9log4cplus6Logger7getRootEv],
                                 [
                                    AC_CHECK_HEADER([log4cplus/log4cplus.h],
                                                    [have_liblog4cplus="yes (autoconf test)"])
                                 ])
                fi

                if test "$have_liblog4cplus" = no; then
                    AC_MSG_ERROR([external liblog4cplus support requested but library not found])
                fi
            fi

            AC_DEFINE(HAVE_LIBLOG4CPLUS, 1, [Have liblog4cplus library])
            AM_CONDITIONAL([COMPILE_LIBLOG4CPLUS], [test "$have_liblog4cplus" != no])
        fi
        AC_LANG_POP([C++])
    ])

