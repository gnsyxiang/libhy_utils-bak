
lib_LTLIBRARIES = libutils.la

AM_CFLAGS  = $(AM_CFLAGS_COM)
AM_CFLAGS += -Wno-error=sign-compare \
             -Wno-error=unused-result \
			 -Wno-error=format= \
			 -Wno-error=unused-variable

libutils_la_LDFLAGS = $(LDFLAGS_COM)

# basic src file
basic_src = signal_wrapper.c \
			thread_wrapper.c \
			utils.c \
			databuf.c \
			time_wrapper.c \
			core_dump.c \
			bitmap_wrapper.c \
			file_wrapper.c

# uart src files
uart_src = uart_wrapper.c \
		   uart_protocol.c

libutils_la_SOURCES  = $(basic_src) \
					   $(uart_src)

all: libutils.la
