bin_PROGRAMS = signal_wrapper \
			   thread_wrapper \
			   uart_wrapper \
			   data_buf_test

AM_CFLAGS  = -I$(top_srcdir)/inc
AM_CFLAGS += -W -Wall -Werror -rdynamic
AM_CFLAGS += -Wno-error=unused-parameter

signal_wrapper_SOURCES = signal_wrapper_test.c
signal_wrapper_LDADD   = ../src/.libs/libutils.a

thread_wrapper_SOURCES = thread_wrapper_test.c
thread_wrapper_LDADD   = ../src/.libs/libutils.a \
						 -lpthread

uart_wrapper_SOURCES = uart_wrapper_test.c
uart_wrapper_LDADD   = ../src/.libs/libutils.a \
					   -lpthread

data_buf_test_SOURCES = data_buf_test.c
data_buf_test_LDADD   = ../src/.libs/libutils.a \
						-lpthread

