
dnl  libhal detection.
m4_define(
        [CHECK_LIBHAL],
        [AC_CACHE_CHECK(
            [for libhal directory],
            [ac_cv_libhal_dir],
            [
                saved_LIBS="$LIBS"
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
                            [#include <stdio.h>
                                #include "hal/hal_thread.h"],
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
            ])
