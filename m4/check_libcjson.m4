
dnl libcjson detection.
m4_define(
        [CHECK_LIBCJSON],
        [
            if test "x$enable_libcjson" = "xyes"; then
                trylibcjsondir=""
                AC_ARG_WITH(
                    [libcjson],
                    [  --with-libcjson=PATH    Specify path to libcjson installation ],
                    [
                        if test "x$withval" != "xno" ; then
                            trylibcjsondir=$withval
                         fi
                    ])

                AC_CACHE_CHECK(
                    [for libcjson directory],
                    [ac_cv_libcjson_dir],
                    [
                        saved_LIBS="$LIBS"
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
                            AC_TRY_LINK(
                                    [#include <stdio.h>
                                        #include "cjson/cJSON.h"],
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
        ])
