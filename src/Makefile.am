SUBDIRS =

lib_LTLIBRARIES = libutils.la

libutils_la_SOURCES = signal_wrapper.c

all: libutils.la

libutils_la_LDFLAGS = -fPIC -version-info @current@:@revision@:@age@

AM_CFLAGS = -I$(top_srcdir)/inc
AM_CFLAGS += -g -O2 -W -Wall -Wno-error=unused-variable
