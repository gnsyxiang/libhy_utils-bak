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

#if 0
#include "uart_wrapper.h"
#include "utils.h"

int main(int argc, const char *argv[])
{
    UartConfig_t uart_config;
    uart_config.num          = UART_NUM_0;
    uart_config.speed        = UART_SPEED_115200;
    uart_config.flow_control = FLOW_CONTROL_NONE;
    uart_config.data_bit     = DATA_BIT_8;
    uart_config.parity_type  = PARITY_NONE;
    uart_config.stop_bit     = STOP_BIT_1;

    int fd = UartInit(&uart_config);
    if (fd < 0) {
        printf("calloc is faild \n");
        return -1;
    }

    char buf[16] = {0};
    int ret;
    while (1) {
        ret = UartRead(fd, buf, 16);
        DumpHexData(buf, ret);
    }

    if (fd > 0) {
        UartFinal(fd);
    }
    
    return 0;
}

#else
#include "uart_protocol.h"

int main(int argc, const char *argv[])
{
    UartProtocolConfig_t config;
    void *handle = UartProtocolInit(&config);

    cmd_t cmd;
    cmd.cmd_type = CMD_TYPE_DOWN;
    cmd.cmd_num  = UART_PROTOCOL_CMD_POWER;

    while (1) {
        sleep(1);
        UartProtocolWriteFrame(handle, &cmd);
    }

    UartProtocolFinal(handle);

    return 0;
}
#endif

