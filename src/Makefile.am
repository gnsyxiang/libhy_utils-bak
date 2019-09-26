lib_LTLIBRARIES = libutils.la

libutils_la_SOURCES  = signal_wrapper.c thread_wrapper.c ringbuf.c utils.c
libutils_la_SOURCES += uart_wrapper.c

libutils_la_LDFLAGS = -fPIC -version-info @current@:@revision@:@age@

AM_CFLAGS = -I$(top_srcdir)/inc
AM_CFLAGS += -W -Wall -Werror -rdynamic
AM_CFLAGS += -Wno-error=sign-compare \
             -Wno-error=unused-result \
			 -Wno-error=format= \
			 -Wno-error=unused-variable

all: libutils.la
