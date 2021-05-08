/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_uart_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    19/11 2020 10:15
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        19/11 2020      create the file
 * 
 *     last modified: 19/11 2020 10:15
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "hy_uart.h"
#include "hy_utils.h"
#include "hy_log.h"

#ifdef USE_DEBUG
    #define ALONE_DEBUG 1
    #define LOG_CATEGORY_TAG "hy_uart"
#endif

typedef struct {
    void    *uart_handle;
} main_context_t;

static main_context_t main_context;

#define UART_DEV_PATH "/dev/ttyUSB0"

static void _read_cb(void *buf, uint32_t len, void *args)
{
    printf("%s", (char *)buf);
}

int main(int argc, char const* argv[])
{
    main_context_t *context = &main_context;

    HyLogCreate(USE_LOG_LEVEL, 1024);

    HyUartConfig_t uart_config;
    memset(&uart_config, '\0', sizeof(uart_config));

    HY_UTILS_COPY(uart_config.dev_path_name, UART_DEV_PATH, DEV_PATH_NAME_LEN);
    uart_config.speed        = HY_UART_SPEED_115200;
    uart_config.flow_control = HY_UART_FLOW_CONTROL_NONE;
    uart_config.data_bit     = HY_UART_DATA_BIT_8;
    uart_config.parity_type  = HY_UART_PARITY_NONE;
    uart_config.stop_bit     = HY_UART_STOP_BIT_1;

    HyUartHandleCb_t handle_cb;
    handle_cb.read_cb   = _read_cb;
    handle_cb.args      = context;

    context->uart_handle = HyUartCreate(&uart_config, &handle_cb);
    if (!context->uart_handle) {
        LOGE("uart create faild \n");
        return -1;
    }

    char buf[12] = {0};
    int cnt = 10;
    while (cnt--) {
        HyUartRead(context->uart_handle, buf, 12);
        sleep(1);
    }

    HyUartDestroy(context->uart_handle);
    HyLogDestory();

    return 0;
}

