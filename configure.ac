#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
m4_include([m4/version.m4])

AC_INIT([libutils], [VERSION_NUM], [gnsyxiang@163.com])
AC_CONFIG_SRCDIR([include/utils_list.h])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_REQUIRE_AUX_FILE([generate-version.sh])

age=VERSION_AGE_NUM
current=VERSION_CURRENT_NUM
revision=VERSION_REVISION_NUM
AC_SUBST(current)
AC_SUBST(revision)
AC_SUBST(age)

AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AM_PROG_AR
LT_INIT

# Checks for libraries.
AC_CHECK_LIB([hal], [HalThreadCreate])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_C_INLINE
AC_TYPE_OFF_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 example/Makefile
                 include/Makefile
                 res/Makefile
                 src/Makefile])

AC_ARG_ENABLE([libcjson],
              [AS_HELP_STRING([--enable-libcjson], [using libcjson for config file] [[default=no]])],
              [AC_DEFINE(HAVE_LIBCJSON_UTILS, 1, [using libcjson for config file])]
              )
AM_CONDITIONAL([COMPILE_WITH_LIBCJSON_UTILS], [test "x$enable_libcjson" = "xyes"])

trylibhaldir=""
AC_ARG_WITH([libhal],
            [  --with-libhal=PATH       Specify path to libhal installation ],
            [if test "x$withval" != "xno" ; then
             trylibhaldir=$withval
             fi]
            )

dnl ------------------------------------------------------
dnl libcjson detection.
if test "x$enable_libcjson" = "xyes"; then
    trylibcjsondir=""
    AC_ARG_WITH([libcjson],
                [  --with-libcjson=PATH     Specify path to libcjson installation ],
                [if test "x$withval" != "xno" ; then
                 trylibcjsondir=$withval
                 fi]
                )

    AC_CACHE_CHECK([for libcjson directory],
                   [ac_cv_libcjson_dir],
                   [saved_LIBS="$LIBS"
                    saved_LDFLAGS="$LDFLAGS"
                    saved_CPPFLAGS="$CPPFLAGS"
                    le_found=no
                    for ledir in $trylibcjsondir "" $prefix /usr/local ; do
                        LDFLAGS="$saved_LDFLAGS"
                        LIBS="$saved_LIBS -lcjson"
                        # Skip the directory if it isn't there.
                        if test ! -z "$ledir" -a ! -d "$ledir" ; then
                            continue;
                        fi
                        if test ! -z "$ledir" ; then
                            if test -d "$ledir/lib" ; then
                                LDFLAGS="-L$ledir/lib $LDFLAGS"
                            else
                                LDFLAGS="-L$ledir $LDFLAGS"
                            fi
                            if test -d "$ledir/include" ; then
                                CPPFLAGS="-I$ledir/include $CPPFLAGS"
                            else
                                CPPFLAGS="-I$ledir $CPPFLAGS"
                            fi
                        fi

                        # Can I compile and link it?
                        AC_TRY_LINK([#include <stdio.h> #include <cJSON.h>],
                                    [cJSON_Parse(NULL);],
                                    [libcjson_linked=yes],
                                    [libcjson_linked=no])
                        if test $libcjson_linked = yes; then
                            if test ! -z "$ledir" ; then
                                ac_cv_libcjson_dir=$ledir
                            else
                                ac_cv_libcjson_dir="(system)"
                            fi
                            le_found=yes
                            break
                        fi
                    done
                    LIBS="$saved_LIBS"
                    LDFLAGS="$saved_LDFLAGS"
                    CPPFLAGS="$saved_CPPFLAGS"
                    if test $le_found = no ; then
                        AC_MSG_ERROR([libcjson is required. If it's already installed, specify its path using --with-libcjson=/dir/])
                    fi
                    ])

    LIBS="$LIBS -lcjson"
    if test $ac_cv_libcjson_dir != "(system)"; then
        if test -d "$ac_cv_libcjson_dir/lib" ; then
            LDFLAGS="-L$ac_cv_libcjson_dir/lib $LDFLAGS"
            le_libdir="$ac_cv_libcjson_dir/lib"
        else
            LDFLAGS="-L$ac_cv_libcjson_dir $LDFLAGS"
            le_libdir="$ac_cv_libcjson_dir"
        fi
        if test -d "$ac_cv_libcjson_dir/include" ; then
            CPPFLAGS="-I$ac_cv_libcjson_dir/include $CPPFLAGS"
        else
            CPPFLAGS="-I$ac_cv_libcjson_dir $CPPFLAGS"
        fi
    fi
fi

dnl ------------------------------------------------------
dnl libhal detection.
AC_CACHE_CHECK(
    [for libhal directory],
    [ac_cv_libhal_dir],
    [saved_LIBS="$LIBS"
     saved_LDFLAGS="$LDFLAGS"
     saved_CPPFLAGS="$CPPFLAGS"
     le_found=no
     for ledir in $trylibhaldir "" $prefix /usr/local ; do
         LDFLAGS="$saved_LDFLAGS"
         LIBS="$saved_LIBS -lhal"
         # Skip the directory if it isn't there.
         if test ! -z "$ledir" -a ! -d "$ledir" ; then
             continue;
         fi
         if test ! -z "$ledir" ; then
             if test -d "$ledir/lib" ; then
                 LDFLAGS="-L$ledir/lib $LDFLAGS"
             else
                 LDFLAGS="-L$ledir $LDFLAGS"
             fi
             if test -d "$ledir/include" ; then
                 CPPFLAGS="-I$ledir/include $CPPFLAGS"
             else
                 CPPFLAGS="-I$ledir $CPPFLAGS"
             fi
         fi
         # Can I compile and link it?
         AC_TRY_LINK(
            [#include <stdio.h> #include <hal_config.h>],
            [HalThreadCreate(NULL);],
            [libhal_linked=yes],
            [libhal_linked=no])
         if test $libhal_linked = yes; then
             if test ! -z "$ledir" ; then
                 ac_cv_libhal_dir=$ledir
             else
                 ac_cv_libhal_dir="(system)"
             fi
             le_found=yes
             break
         fi
     done
     LIBS="$saved_LIBS"
     LDFLAGS="$saved_LDFLAGS"
     CPPFLAGS="$saved_CPPFLAGS"
     if test $le_found = no ; then
        AC_MSG_ERROR([libhal is required. If it's already installed, specify its path using --with-libhal=/dir/])
     fi
    ])

LIBS="$LIBS -lhal"
if test $ac_cv_libhal_dir != "(system)"; then
    if test -d "$ac_cv_libhal_dir/lib" ; then
        LDFLAGS="-L$ac_cv_libhal_dir/lib $LDFLAGS"
        le_libdir="$ac_cv_libhal_dir/lib"
    else
        LDFLAGS="-L$ac_cv_libhal_dir $LDFLAGS"
        le_libdir="$ac_cv_libhal_dir"
    fi
    if test -d "$ac_cv_libhal_dir/include" ; then
        CPPFLAGS="-I$ac_cv_libhal_dir/include $CPPFLAGS"
    else
        CPPFLAGS="-I$ac_cv_libhal_dir $CPPFLAGS"
    fi
fi

AC_OUTPUT

