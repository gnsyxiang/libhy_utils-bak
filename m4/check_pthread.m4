dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    check_pthread.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    21/10 2021 15:20
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        21/10 2021      create the file
dnl 
dnl     last modified: 21/10 2021 15:20
dnl ===============================================================

# CHECK_PTHREAD()
# --------------------------------------------------------------
# check pthread

AC_DEFUN([CHECK_PTHREAD], [
    AC_MSG_CHECKING([whether to use POSIX threads for threaded resolver])

    AC_ARG_ENABLE(pthreads,
[AS_HELP_STRING([--enable-pthreads], [Enable POSIX threads (default for threaded resolver)])
AS_HELP_STRING([--disable-pthreads],[Disable POSIX threads])],
        [
            case "$enableval" in
                no) AC_MSG_RESULT(no)
                    want_pthreads=no
                ;;
            *)  AC_MSG_RESULT(yes)
                want_pthreads=yes
                ;;
            esac
        ],
        [
            AC_MSG_RESULT(auto)
            want_pthreads=auto
        ]
    )

    if test "$want_pthreads" != "no"; then
      AC_CHECK_HEADER(pthread.h,
        [ AC_DEFINE(HAVE_PTHREAD_H, 1, [if you have <pthread.h>])
          save_CFLAGS="$CFLAGS"
          save_LIBS="$LIBS"

          LIBS=
          AC_CHECK_FUNC(pthread_create, [USE_THREADS_POSIX=1] )
          LIBS="$save_LIBS"

          if test "$USE_THREADS_POSIX" != "1"
          then
            PTHREAD="-lpthread"
            AC_SUBST(PTHREAD)
            AC_CHECK_LIB(pthread, pthread_create,
                         [USE_THREADS_POSIX=1],
                         [ CFLAGS="$save_CFLAGS"])
          fi

          if test "x$USE_THREADS_POSIX" = "x1"
          then
            LIBS_PRIVAATE="$LIBS_PRIVAATE $PTHREAD"
            AC_DEFINE(USE_THREADS_POSIX, 1, [if you want POSIX threaded DNS lookup])
          fi
        ])
    fi

    AM_CONDITIONAL(COMPILE_THREADS_POSIX, [test "x$USE_THREADS_POSIX" = "x1"])

    if test "x$USE_THREADS_POSIX" != "x1"; then
      if test "$want_pthreads" = "yes"; then
        AC_MSG_ERROR([--enable-pthreads but pthreads was not found])
      fi
    fi
])

