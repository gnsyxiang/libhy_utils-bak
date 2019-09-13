bin_PROGRAMS = signal_wrapper thread_wrapper

signal_wrapper_SOURCES = signal_wrapper_test.c
signal_wrapper_LDADD   = ../src/.libs/libutils.a

thread_wrapper_SOURCES = thread_wrapper_test.c
thread_wrapper_LDADD   = ../src/.libs/libutils.a
thread_wrapper_LDADD  += -lpthread

AM_CFLAGS  = -I$(top_srcdir)/inc
AM_CFLAGS += -g -O2 -W -Wall
AM_CFLAGS += -Wno-error=unused-variable
