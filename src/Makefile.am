lib_LTLIBRARIES = libutils.la

AM_CFLAGS = -I$(top_srcdir)/inc
AM_CFLAGS += -W -Wall -Werror -rdynamic \
			 -Wno-error=sign-compare \
             -Wno-error=unused-result \
			 -Wno-error=format= \
			 -Wno-error=unused-variable

libutils_la_LDFLAGS = -fPIC \
					  -version-info @current@:@revision@:@age@

# basic src file
basic_src = signal_wrapper.c \
			thread_wrapper.c \
			ringbuf.c \
			utils.c \
			data_buf.c

# uart src files
uart_src = uart_wrapper.c

libutils_la_SOURCES  = $(basic_src) \
					   $(uart_src)

all: libutils.la
