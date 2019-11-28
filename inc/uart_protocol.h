/**
 *
 * Release under GPL-3.0.
 * 
 * @file    uart_protocol.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/11 2019 10:06
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/11 2019      create the file
 * 
 *     last modified: 26/11 2019 10:06
 */
#ifndef __LIBUTILS_INC_UART_PROTOCOL_H_
#define __LIBUTILS_INC_UART_PROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBUTILS_INC_UART_PROTOCOL_GB
#define LIBUTILS_INC_UART_PROTOCOL_EX extern
#else
#define LIBUTILS_INC_UART_PROTOCOL_EX
#endif

#include <sys/types.h>

typedef enum {
    UART_PROTOCOL_CMD_POWER,
    UART_PROTOCOL_CMD_QUERY,
    UART_PROTOCOL_CMD_MAX,
} UartProtocolCmdSetting_t;

typedef enum {
    UART_PROTOCOL_INTERFACE_SYNC,
    UART_PROTOCOL_INTERFACE_ASYNC,
    UART_PROTOCOL_INTERFACE_MAX,
} UartProtocolInterfaceType_t;

typedef void (*UartProtocolReadCB_t)(const char *buf, size_t len);

typedef struct {
    UartProtocolInterfaceType_t interface_type; 
    UartProtocolReadCB_t        read_cb;
} UartProtocolConfig_t;

void *UartProtocol_Init(UartProtocolConfig_t *config);
int UartProtocol_Final(void *handle);

int UartProtocol_WriteFrame(void *handle, UartProtocolCmdSetting_t cmd_setting);
int UartProtocol_ReadFrame(void *handle, char * const buf);

#ifdef __cplusplus
}
#endif

#endif // __LIBUTILS_INC_UART_PROTOCOL_H_

