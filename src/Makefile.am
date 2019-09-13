lib_LTLIBRARIES = libutils.la

libutils_la_SOURCES = signal_wrapper.c thread_wrapper.c

libutils_la_LDFLAGS = -fPIC -version-info @current@:@revision@:@age@

AM_CFLAGS = -I$(top_srcdir)/inc
AM_CFLAGS += -g -O2 -W -Wall
# AM_CFLAGS += -Wno-error=unused-variable

all: libutils.la
