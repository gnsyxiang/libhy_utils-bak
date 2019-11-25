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
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "file_wrapper.h"

#define LIBUTILS_INC_UART_WRAPPER_GB
#include "uart_wrapper.h"
#undef LIBUTILS_INC_UART_WRAPPER_GB

#define UART_READ_VMIN_LEN      (16)
#define UART_READ_VTIME_100MS   (10)
                                
#define uart_log(fmt, ...) \
    printf("<%s:%d, result: %s> " fmt, \
           __func__, __LINE__, strerror(errno), ##__VA_ARGS__);

static int _set_param(int fd, UartConfig_t *uart_config)
{
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
		case STOP_BIT_1: options.c_cflag &= ~CSTOPB;             break;
		case STOP_BIT_2: options.c_cflag |= CSTOPB;              break;
        case STOP_BIT_MAX: uart_log("the stop_bit is error \n"); break;
	}

    // 设置数据流控
    switch (uart_config->flow_control) {
        case FLOW_CONTROL_NONE:     options.c_cflag &= ~CRTSCTS;             break;
        case FLOW_CONTROL_HARDWARE: options.c_cflag |= CRTSCTS;              break;
        case FLOW_CONTROL_SOFT:     options.c_cflag |= IXON | IXOFF | IXANY; break;
        case FLOW_CONTROL_MAX: uart_log("the flow_control is error \n");     break;
    }

	// read阻塞条件: wait time and minmum number of "bytes"
	options.c_cc[VTIME] = UART_READ_VTIME_100MS; // wait for 0.1s
    options.c_cc[VMIN]  = UART_READ_VMIN_LEN;     // read at least 1 byte

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

static int _init_uart(UartConfig_t *uart_config)
{
    char *uart_num_2_name[UART_NUM_MAX] = {
        "/dev/ttyUSB0",
        "/dev/ttyUSB1",
    };

    int flags = O_RDWR | O_NOCTTY;
    // int flags = O_RDWR | O_NOCTTY | O_NDELAY;
    int fd = open(uart_num_2_name[uart_config->num], flags);
    if (fd == -1) {
        uart_log("open %s faild \n", uart_num_2_name[uart_config->num]);
        return -1;
    }
    uart_log("open %s successful \n", uart_num_2_name[uart_config->num]);

    if (_set_param(fd, uart_config)) {
        uart_log("_set_param faild \n");
        close(fd);
        return -1;
    }
    uart_log("set uart successful \n");

    return fd;
}

int UartInit(UartConfig_t *uart_config)
{
    if (!uart_config) {
        uart_log("the uart_config is NULL");
        return -1;
    }

    int fd = -1;
    return (fd = _init_uart(uart_config) > 0) ? fd : -1;
}

void UartFinal(int fd)
{
    if (fd > 0) {
        close(fd);
        uart_log("close uart successful \n");
    }
}

#define _judge_param_common(fd, buf, len)           \
    if (fd < 0) {                                   \
        uart_log("the fd is error \n");             \
        return -1;                                  \
    }                                               \
    if (buf == NULL) {                              \
        uart_log("the buf is NULL \n");             \
        return -2;                                  \
    }                                               \
    if (len <= 0) {                                 \
        uart_log("the len is error \n");            \
        return -2;                                  \
    }

ssize_t UartWrite(int fd, void *buf, size_t len)
{
    _judge_param_common(fd, buf, len);
    return FileWrite(fd, buf, len);
}

ssize_t UartRead(int fd, void *buf, size_t len)
{
    _judge_param_common(fd, buf, len);
    return FileRead(fd, buf, len);
}

