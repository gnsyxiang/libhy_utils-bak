#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])


AC_CONFIG_MACRO_DIRS([m4])
# AM_ACLOCAL_INCLUDE([m4])
m4_include([m4/version.m4])

AC_INIT([libutils], [VERSION_NUM], [gnsyxiang@163.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_CONFIG_SRCDIR([inc/signal_wrapper.h])
AC_CONFIG_HEADERS([config.h])

current=VERSION_CURRENT_NUM
revision=VERSION_REVISION_NUM
age=VERSION_AGE_NUM
AC_SUBST(current)
AC_SUBST(revision)
AC_SUBST(age)
#
# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_AR
LT_INIT

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([strdup])

AC_CONFIG_FILES([Makefile
                 inc/Makefile
                 res/Makefile
                 res/script/Makefile
                 sample/Makefile
                 src/Makefile])
AC_OUTPUT
