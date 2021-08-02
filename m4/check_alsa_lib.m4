dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    check_alsa_lib.m4
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

# CHECK_ALSA_LIB()
# --------------------------------------------------------------
# check libalsa_lib

AC_DEFUN([CHECK_ALSA_LIB], [

    AC_ARG_ENABLE([alsa_lib],
        [AS_HELP_STRING([--disable-alsa_lib], [disable support for alsa_lib])],
            [], [enable_alsa_lib=yes])

    case "$enable_alsa_lib" in
        yes)
            have_alsa_lib=no

            case "$PKG_CONFIG" in
                '') ;;
                *)
                    ALSA_LIB_LIBS=`$PKG_CONFIG --libs alsa_lib 2>/dev/null`

                    case "$ALSA_LIB_LIBS" in
                        '') ;;
                        *)
                            ALSA_LIB_LIBS="$ALSA_LIB_LIBS"
                            have_alsa_lib=yes
                        ;;
                    esac

                    ALSA_LIB_INCS=`$PKG_CONFIG --cflags alsa_lib 2>/dev/null`
                ;;
            esac

            case "$have_alsa_lib" in
                yes) ;;
                *)
                    save_LIBS="$LIBS"
                    LIBS=""
                    ALSA_LIB_LIBS=""

                    # clear cache
                    unset ac_cv_search_snd_pcm_close
                    AC_SEARCH_LIBS([snd_pcm_close], [alsa_lib],
                            [have_alsa_lib=yes
                                ALSA_LIB_LIBS="$LIBS"],
                            [have_alsa_lib=no],
                            [])
                    LIBS="$save_LIBS"
                ;;
            esac

            CPPFLAGS_SAVE=$CPPFLAGS
            CPPFLAGS="$CPPFLAGS $ALSA_LIB_INCS"
            AC_CHECK_HEADERS([alsa/asoundlib.h], [], [have_alsa_lib=no])

            CPPFLAGS=$CPPFLAGS_SAVE
            AC_SUBST(ALSA_LIB_INCS)
            AC_SUBST(ALSA_LIB_LIBS)

            case "$have_alsa_lib" in
                yes)
                    AC_DEFINE(HAVE_ALSA_LIB, 1, [Define if the system has alsa_lib])
                ;;
                *)
                    AC_MSG_ERROR([alsa_lib is a must but can not be found. You should add the \
directory containing `alsa_lib.pc' to the `PKG_CONFIG_PATH' environment variable, \
or set `CPPFLAGS' and `LDFLAGS' directly for alsa_lib, or use `--disable-alsa_lib' \
to disable support for alsa_lib encryption])
                ;;
            esac
        ;;
    esac

    # check if we have and should use alsa_lib
    AM_CONDITIONAL(COMPILE_ALSA_LIB, [test "$enable_alsa_lib" != "no" && test "$have_alsa_lib" = "yes"])
])

