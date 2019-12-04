
AM_CFLAGS  = $(AM_CFLAGS_COM)
AM_CFLAGS += -Wno-error=unused-parameter

LDFLAGS_COMMON = ../src/.libs/libutils.a

bin_PROGRAMS = signal_wrapper \
			   thread_wrapper \
			   uart_test \
			   data_buf_test

data_buf_test_SOURCES = data_buf_test.c
data_buf_test_LDADD   = $(LDFLAGS_COMMON) -lpthread

uart_test_SOURCES = uart_test.c
uart_test_LDADD   = $(LDFLAGS_COMMON) -lpthread

thread_wrapper_SOURCES = thread_wrapper_test.c
thread_wrapper_LDADD   = $(LDFLAGS_COMMON) -lpthread

signal_wrapper_SOURCES = signal_wrapper_test.c
signal_wrapper_LDADD   = $(LDFLAGS_COMMON)

