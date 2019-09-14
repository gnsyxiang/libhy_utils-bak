/**
 * 
 * Release under GPLv2.
 * 
 * @file    uart_wrapper_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    14/09 2019 14:14
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        14/09 2019      create the file
 * 
 *     last modified: 14/09 2019 14:14
 */
#include <stdio.h>
#include <unistd.h>

#include "uart_wrapper.h"

static void _uart_dump_data(char *buf, size_t len)
{
    int i;
    printf("len: %d ---->> ", len);
    for (i = 0; i < len; i++) {
        printf("%02x ", (unsigned char )buf[i]);
    }
    printf("\n");
}

static void _uart_read_cb(char *buf, size_t len)
{
    _uart_dump_data(buf, len);
}

int main(int argc, const char *argv[])
{
    UartConfig_t uart_config;
    uart_config.num          = UART_NUM_1;
    uart_config.speed        = UART_SPEED_115200;
    uart_config.flow_control = FLOW_CONTROL_NONE;
    uart_config.data_bit     = DATA_BIT_8;
    uart_config.parity_type  = PARITY_NONE;
    uart_config.stop_bit     = STOP_BIT_1;
    uart_config.read_cb      = _uart_read_cb;

    void *handle = UartInit(&uart_config);

    char haha[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    UartWrite(handle, haha, sizeof(haha));

    while (1) {
        sleep(1);
    }
    
    UartFinal(handle);

    return 0;
}
