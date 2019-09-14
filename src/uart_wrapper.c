/**
 * 
 * Release under GPLv2.
 * 
 * @file    uart_wrapper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    14/09 2019 12:41
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        14/09 2019      create the file
 * 
 *     last modified: 14/09 2019 12:41
 */
#include <stdio.h>
#include <errno.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "thread_wrapper.h"

#include "uart_wrapper.h"

#define UART_DEBUG
#define UART_READ_VMIN_LEN (16)
#define uart_log(fmt, ...) \
    printf("<%s:%d, result: %s> " fmt, \
            __func__, __LINE__, strerror(errno), ##__VA_ARGS__);

typedef struct {
    int             fd;
    UartRead_cb_t   read_cb;
} UartState_t;
#define UART_STATE_LEN (sizeof(UartState_t))

static int _set_param(UartState_t *uart_state, UartConfig_t *uart_config)
{
    int fd = uart_state->fd;
    struct termios options;

    if (tcgetattr(fd, &options)) {
        uart_log("tcgetattr faild \n");
        return -1;
    }
    
    // 配置为原始模式 (两种方式任选其一)
#if 0
     cfmakeraw(&options);
#else
    options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP| INLCR | IGNCR | ICRNL | IXON);
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    options.c_cflag &= ~(CSIZE | PARENB);
    options.c_cflag |= CS8;
#endif

	options.c_cflag |= (CLOCAL | CREAD); // CLOCAL和CREAD分别用于本地连接和接收使能

    // 设置波特率
    speed_t speed_2_speed[UART_SPEED_MAX] = {B4800, B9600, B115200};
	cfsetispeed(&options, speed_2_speed[uart_config->speed]);
	cfsetospeed(&options, speed_2_speed[uart_config->speed]);

    // 设置数据位
    int data_bit_2_bit[DATA_BIT_MAX] = {CS5, CS6, CS7, CS8};
	options.c_cflag &= ~CSIZE;  // 清零
    options.c_cflag |= data_bit_2_bit[uart_config->data_bit];

    // 设置校验位
	switch (uart_config->parity_type) {
		case PARITY_NONE:
			options.c_cflag &= ~PARENB;
			options.c_iflag &= ~INPCK;
			break;
		case PARITY_ODD:
			options.c_cflag |= (PARENB | PARODD);
			options.c_iflag |= INPCK;
			break;
		case PARITY_EVEN:
			options.c_cflag |= PARENB;
			options.c_cflag &= ~PARODD;
			options.c_iflag |= INPCK;
			break;
        case PARITY_MAX:
            uart_log("the parity_type is error \n");
            break;
	}

    // 设置停止位
	switch (uart_config->stop_bit) {
		case STOP_BIT_1: options.c_cflag &= ~CSTOPB; break;
		case STOP_BIT_2: options.c_cflag |= CSTOPB;  break;
        case STOP_BIT_MAX: uart_log("the stop_bit is error \n"); break;
	}

    // 设置数据流控
    switch (uart_config->flow_control) {
        case FLOW_CONTROL_NONE:     options.c_cflag &= ~CRTSCTS;             break;
        case FLOW_CONTROL_HARDWARE: options.c_cflag |= CRTSCTS;              break;
        case FLOW_CONTROL_SOFT:     options.c_cflag |= IXON | IXOFF | IXANY; break;
        case FLOW_CONTROL_MAX: uart_log("the flow_control is error \n");       break;
    }

	// read阻塞条件: wait time and minmum number of "bytes"
	options.c_cc[VTIME]= 0;                     // wait for 0.1s
    options.c_cc[VMIN]= UART_READ_VMIN_LEN;     // read at least 1 byte

    // TCIFLUSH刷清输入队列
    // TCOFLUSH刷清输出队列
    // TCIOFLUSH刷清输入、输出队列
    tcflush (fd, TCIOFLUSH);

    // TCSANOW立即生效,
    // TCSADRAIN：Wait until everything has been transmitted；
    // TCSAFLUSH：Flush input and output buffers and make the change
	if (tcsetattr(fd, TCSANOW, &options) != 0) {
		uart_log("tty set error!\n");
		return -1;
	}
	return 0;
}

static int _init_uart(UartState_t *uart_state, UartConfig_t *uart_config)
{
    char *uart_num_2_name[UART_NUM_MAX] = {
        "/dev/ttyUSB7",
        // "/dev/ttyS1",
        "/dev/ttyS2",
    };

    int flags = O_RDWR | O_NOCTTY;
    // int flags = O_RDWR | O_NOCTTY | O_NDELAY;
    uart_state->fd = open(uart_num_2_name[uart_config->num], flags);
    if (uart_state->fd <= 0) {
        uart_log("open %s faild \n", uart_num_2_name[uart_config->num]);
        return -1;
    }
    uart_log("open %s successful \n", uart_num_2_name[uart_config->num]);

    if (_set_param(uart_state, uart_config)) {
        uart_log("_set_param faild \n");
        close(uart_state->fd);
        return -1;
    }
    uart_log("set uart successful \n");

    return 0;
}

#ifdef UART_DEBUG
static void _uart_dump_data(char *buf, size_t len)
{
    int i;
    printf("len: %d ---->> ", len);
    for (i = 0; i < len; i++) {
        printf("%02x ", (unsigned char )buf[i]);
    }
    printf("\n");
}
#endif

static void *_uart_write_loop(void *args)
{
    while (1) {
        sleep(1);
    }
    return NULL;
}

static void *_uart_read_loop(void *args)
{
    UartState_t *uart_state = (UartState_t *)args;
    char buf[UART_READ_VMIN_LEN];
    int ret;

    while (1) {
        ret = read(uart_state->fd, buf, UART_READ_VMIN_LEN);
#ifdef UART_DEBUG
        _uart_dump_data(buf, ret);
#endif
        if (uart_state->read_cb) {
            uart_state->read_cb(buf, ret);
        }
    }
    return NULL;
}

static inline void _create_thread(ThreadLoop_t thread_loop, void *args)
{
    ThreadParam_t thread_param;
    thread_param.id          = NULL;
    thread_param.thread_loop = thread_loop;
    thread_param.args        = args;
    CreateAttachedThread(&thread_param);
}

void *UartInit(UartConfig_t *uart_config)
{
    if (!uart_config) {
        uart_log("the uart_config is NULL");
        return NULL;
    }

    UartState_t *uart_state = calloc(1, UART_STATE_LEN);
    if (!uart_state) {
        uart_log("calloc faild \n");
        return NULL;
    }

    if (_init_uart(uart_state, uart_config)) {
        uart_log("_init_uart faild \n");
        free(uart_state);
        return NULL;
    }

    uart_state->read_cb = uart_config->read_cb;

    _create_thread(_uart_write_loop, uart_state);
    _create_thread(_uart_read_loop, uart_state);

    return (uart_state);
}

void UartFinal(void *handle)
{
    free(handle);
}

size_t file_write(int fd, const void *buf, size_t len)
{
    int ret;
    size_t nleft;
    const char *ptr;

    ptr = buf;
    nleft = len;

    while (nleft > 0) {
        if ((ret = write(fd, ptr, nleft)) <= 0) {
            if (ret < 0 && errno == EINTR) {
                ret = 0;
            }
            else {
                return -1;
            }
        }

        nleft -= ret;
        ptr   += ret;
    }

    return len;
}

int UartWrite(void *handle, void *buf, size_t len)
{
    if (!handle || !buf) {
        uart_log("the param is NULL \n");
        return -1;
    }
    int fd = ((UartState_t*)handle)->fd;

    file_write(fd, buf, len);
    return 0;
}
