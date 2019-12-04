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

#include "utils.h"

typedef enum {
    UART_PROTOCOL_VAL_ON,
    UART_PROTOCOL_VAL_OFF,
    UART_PROTOCOL_VAL_NONE,

    UART_PROTOCOL_VAL_MAX,
} UartProtocolCmdVal_t;

typedef enum {
    UART_PROTOCOL_CMD_POWER,
    UART_PROTOCOL_CMD_QUERY,

    UART_PROTOCOL_CMD_MAX,
} UartProtocolCmdSetting_t;

typedef struct {
    UartProtocolCmdSetting_t    cmd;
    int                         val;
} UartProtocolCmd_t;

typedef struct {
    int power;
} UartProtocolState_t;

typedef void (*UartProtocolReadFrameCB_t)(const UartProtocolState_t * const state);

typedef struct {
    UartProtocolReadFrameCB_t       read_cb;
} UartProtocolConfig_t;

LIBUTILS_INC_UART_PROTOCOL_EX void *UartProtocolInit(UartProtocolConfig_t *config);
LIBUTILS_INC_UART_PROTOCOL_EX int UartProtocolFinal(void *handle);

LIBUTILS_INC_UART_PROTOCOL_EX int UartProtocolWriteFrame(void *handle, UartProtocolCmd_t *cmd);

#ifdef __cplusplus
}
#endif

#endif // __LIBUTILS_INC_UART_PROTOCOL_H_
