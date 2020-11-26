#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
m4_include([m4/version.m4])

AC_INIT([libutils], [VERSION_NUM], [gnsyxiang@163.com])
AC_CONFIG_SRCDIR([include/hy_log.h])
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
AM_PROG_AR

LT_INIT

AC_ARG_ENABLE(
              [libcjson],
              [AS_HELP_STRING([--enable-libcjson], [using libcjson for config file] [[default=no]])],
              [AC_DEFINE(HAVE_LIBCJSON_UTILS, 1, [using libcjson for config file])]
              )
AM_CONDITIONAL([COMPILE_WITH_LIBCJSON_UTILS], [test "x$enable_libcjson" = "xyes"])

trylibhaldir=""
AC_ARG_WITH(
            [libhal],
            [  --with-libhal=PATH       Specify path to libhal installation ],
            [if test "x$withval" != "xno" ; then
             trylibhaldir=$withval
             fi]
            )

m4_include([m4/check_libcjson.m4])
m4_include([m4/check_libhal.m4])
CHECK_LIBHAL
CHECK_LIBCJSON

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
# AC_PROG_RANLIB
AC_PROG_MKDIR_P

# Checks for libraries.
AC_CHECK_LIB([cjson], [cJSON_Parse])
AC_CHECK_LIB([hal], [HalThreadCreate])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stddef.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 example/Makefile
                 include/Makefile
                 res/Makefile
                 src/Makefile])

AC_OUTPUT
