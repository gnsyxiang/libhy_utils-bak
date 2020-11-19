/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_uart.c
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
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "hy_uart.h"

#include "hy_type.h"
#include "hy_file.h"
#include "hy_log.h"

#ifdef USE_DEBUG
    #define ALONE_DEBUG 1
    #define LOG_CATEGORY_TAG "hy_uart"
#endif

typedef void *(*HyPthreadLoopCb_t)(void *args);
typedef struct {
    pthread_t           pthread_id;
    HyPthreadLoopCb_t   loop_cb;
    int                 loop_flag;
} HyPthread_t;

void HyPthreadCreate(HyPthread_t *pthread, void *args)
{
    pthread->loop_flag = 1;
    pthread_create(&pthread->pthread_id, NULL, pthread->loop_cb, args);
}

void HyPthreadDestroy(HyPthread_t *pthread)
{
    pthread->loop_flag = 0;
    if (0 != pthread_join(pthread->pthread_id, NULL)) {
        LOGE("pthread_join error \n");
    }
}

typedef struct {
    HyUartHandleCb_t    handle_cb;
    int                 fd;
    HyPthread_t         read_thread;
} hy_uart_context_t;

static int _set_param(int fd, HyUartConfig_t *uart_config)
{
    struct termios options;

    if (tcgetattr(fd, &options)) {
        LOGE("tcgetattr faild \n");
        return ERR_FAILD;
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

    // CLOCAL和CREAD分别用于本地连接和接收使能
    options.c_cflag |= (CLOCAL | CREAD); 

    // 设置波特率
    speed_t speed_2_speed[HY_UART_SPEED_MAX] = {B4800, B9600, B115200};
    cfsetispeed(&options, speed_2_speed[uart_config->speed]);
    cfsetospeed(&options, speed_2_speed[uart_config->speed]);

    // 设置数据位
    int data_bit_2_bit[HY_UART_DATA_BIT_MAX] = {CS5, CS6, CS7, CS8};
    options.c_cflag &= ~CSIZE;  // 清零
    options.c_cflag |= data_bit_2_bit[uart_config->data_bit];

    // 设置校验位
    switch (uart_config->parity_type) {
        case HY_UART_PARITY_NONE:
            options.c_cflag &= ~PARENB;
            options.c_iflag &= ~INPCK;
            break;
        case HY_UART_PARITY_ODD:
            options.c_cflag |= (PARENB | PARODD);
            options.c_iflag |= INPCK;
            break;
        case HY_UART_PARITY_EVEN:
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_iflag |= INPCK;
            break;
        case HY_UART_PARITY_MAX:
            LOGE("the parity_type is error \n");
            break;
    }

    // 设置停止位
    switch (uart_config->stop_bit) {
        case HY_UART_STOP_BIT_1:    options.c_cflag &= ~CSTOPB;         break;
        case HY_UART_STOP_BIT_2:    options.c_cflag |= CSTOPB;          break;
        case HY_UART_STOP_BIT_MAX: LOGE("the stop_bit is error \n");    break;
    }

    // 设置数据流控
    switch (uart_config->flow_control) {
        case HY_UART_FLOW_CONTROL_NONE:     options.c_cflag &= ~CRTSCTS;             break;
        case HY_UART_FLOW_CONTROL_HARDWARE: options.c_cflag |= CRTSCTS;              break;
        case HY_UART_FLOW_CONTROL_SOFT:     options.c_cflag |= IXON | IXOFF | IXANY; break;
        case HY_UART_FLOW_CONTROL_MAX: LOGE("the flow_control is error \n");     break;
    }

    // open的时候没有设置O_NONBLOCK或O_NDELAY，下面两个参数起效
    // 再接收到第一个字节后，开始计算超时时间
    // 接收到VMIN字节后，read直接返回
    // 在VTIME时间内，没有接收到VIMIN字节，read也直接返回
    options.c_cc[VTIME] = UART_READ_VTIME_100MS;    // 百毫秒, 是一个unsigned char变量
    options.c_cc[VMIN]  = UART_READ_VMIN_LEN;       // read at least 1 byte

    // TCIFLUSH刷清输入队列
    // TCOFLUSH刷清输出队列
    // TCIOFLUSH刷清输入、输出队列
    tcflush (fd, TCIOFLUSH);

    // TCSANOW立即生效,
    // TCSADRAIN：Wait until everything has been transmitted；
    // TCSAFLUSH：Flush input and output buffers and make the change
    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        LOGE("tty set error!\n");
        return ERR_FAILD;
    }

    return ERR_OK;
}

static int _init_uart(HyUartConfig_t *uart_config)
{
    // O_NONBLOCK和O_NDELAY区别:
    // 它们的差别在于设立O_NDELAY会使I/O函式马上回传0，但是又衍生出一个问题，
    // 因为读取到档案结尾时所回传的也是0，这样无法得知是哪中情况；
    // 因此，O_NONBLOCK就产生出来，它在读取不到数据时会回传-1，并且设置errno为EAGAIN。
    int flags = O_RDWR | O_NOCTTY;
    // int flags = O_RDWR | O_NOCTTY | O_NONBLOCK;
    // int flags = O_RDWR | O_NOCTTY | O_NDELAY;

    int fd = open(uart_config->dev_path_name, flags);
    if (-1 == fd) {
        LOGE("open %s faild \n", uart_config->dev_path_name);
        return -1;
    }

    if (ERR_OK != _set_param(fd, uart_config)) {
        LOGE("_set_param faild \n");
        close(fd);
        return -1;
    }

    return fd;
}

static void *_read_loop_cb(void *args)
{
#define BUF_LEN (1024)
    char buf[BUF_LEN] = {0};
    uint32_t ret;

    hy_uart_context_t *context = args;
    HyPthread_t *pthread = &context->read_thread;
    HyUartHandleCb_t *handle_cb = &context->handle_cb;

    while (pthread->loop_flag) {
        memset(buf, '\0', BUF_LEN);
        ret = read(context->fd, buf, BUF_LEN);
        if (ret > 0 && handle_cb->read_cb) {
            handle_cb->read_cb(buf, ret, handle_cb->args);
        }
    }

    LOGD("read loop exit \n");
    return NULL;
}

// --------------------------------------------------------

uint32_t HyUartWrite(void *handle, void *buf, uint32_t len)
{
    if (!handle || !buf || 0 == len) {
        LOGE("the param is NULL \n");
        return -1;
    }

    hy_uart_context_t *context = handle;
    return HyFileWrite(context->fd, buf, len);
}

uint32_t HyUartRead(void *handle, void *buf, uint32_t len)
{
    if (!handle || !buf || 0 == len) {
        LOGE("the param is NULL \n");
        return -1;
    }

    hy_uart_context_t *context = handle;
    if (context->handle_cb.read_cb) {
        LOGE("use callback instand of \n");
        return -1;
    } else {
        return HyFileRead(context->fd, buf, len);
    }
}

void *HyUartCreate(HyUartConfig_t *uart_config, HyUartHandleCb_t *handle_cb)
{
    if (!uart_config) {
        LOGE("the param is NULL \n");
        return NULL;
    }

    hy_uart_context_t *context = calloc(1, sizeof(*context));
    if (!context) {
        LOGE("calloc faild \n");
        return NULL;
    }

    context->fd = _init_uart(uart_config);
    if (-1 == context->fd) {
        free(context);
        context = NULL;
        LOGE("init uart faild \n");
        return NULL;
    }

    if (handle_cb) {
        memcpy(&context->handle_cb, handle_cb, sizeof(*handle_cb));

        HyPthread_t *pthread = &context->read_thread;
        pthread->loop_cb = _read_loop_cb;
        HyPthreadCreate(pthread, context);
    }

    return context;
}

void HyUartDestroy(void *handle)
{
    if (!handle) {
        LOGE("the param is NULL \n");
        return ;
    }

    hy_uart_context_t *context = handle;

    HyPthreadDestroy(&context->read_thread);

    if (context->fd) {
        close(context->fd);
    }

    if (context) {
        free(context);
    }
}

