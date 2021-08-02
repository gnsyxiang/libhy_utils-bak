dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    check_asound.m4
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

# CHECK_ASOUND()
# --------------------------------------------------------------
# check libasound

AC_DEFUN([CHECK_ASOUND], [

    AC_ARG_ENABLE([asound],
        [AS_HELP_STRING([--disable-asound], [disable support for asound])],
            [], [enable_asound=yes])

    case "$enable_asound" in
        yes)
            have_asound=no

            case "$PKG_CONFIG" in
                '') ;;
                *)
                    ASOUND_LIBS=`$PKG_CONFIG --libs alsa 2>/dev/null`

                    case "$ASOUND_LIBS" in
                        '') ;;
                        *)
                            ASOUND_LIBS="$ASOUND_LIBS"
                            have_asound=yes
                        ;;
                    esac

                    ASOUND_INCS=`$PKG_CONFIG --cflags alsa 2>/dev/null`
                ;;
            esac

            case "$have_asound" in
                yes) ;;
                *)
                    save_LIBS="$LIBS"
                    LIBS=""
                    ASOUND_LIBS=""

                    # clear cache
                    unset ac_cv_search_snd_pcm_close
                    AC_SEARCH_LIBS([snd_pcm_close], [asound],
                            [have_asound=yes
                                ASOUND_LIBS="$LIBS"],
                            [have_asound=no],
                            [])
                    LIBS="$save_LIBS"
                ;;
            esac

            CPPFLAGS_SAVE=$CPPFLAGS
            CPPFLAGS="$CPPFLAGS $ASOUND_INCS"
            AC_CHECK_HEADERS([alsa/asoundlib.h], [], [have_asound=no])

            CPPFLAGS=$CPPFLAGS_SAVE
            AC_SUBST(ASOUND_INCS)
            AC_SUBST(ASOUND_LIBS)

            case "$have_asound" in
                yes)
                    AC_DEFINE(HAVE_ASOUND, 1, [Define if the system has asound])
                ;;
                *)
                    AC_MSG_ERROR([asound is a must but can not be found. You should add the \
directory containing `asound.pc' to the `PKG_CONFIG_PATH' environment variable, \
or set `CPPFLAGS' and `LDFLAGS' directly for asound, or use `--disable-asound' \
to disable support for asound encryption])
                ;;
            esac
        ;;
    esac

    # check if we have and should use asound
    AM_CONDITIONAL(COMPILE_ASOUND, [test "$enable_asound" != "no" && test "$have_asound" = "yes"])
])

