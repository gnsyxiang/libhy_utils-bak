SUBDIRS =

bin_PROGRAMS = signal_wrapper

signal_wrapper_SOURCES = signal_wrapper_test.c
signal_wrapper_LDADD=../src/.libs/libutils.a

AM_CFLAGS = -I$(top_srcdir)/inc
AM_CFLAGS += -g -O2 -W -Wall
