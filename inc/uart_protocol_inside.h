/**
 *
 * Release under GPL-3.0.
 * 
 * @file    uart_protocol_inside.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    04/12 2019 14:56
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        04/12 2019      create the file
 * 
 *     last modified: 04/12 2019 14:56
 */
#ifndef __LIBUTILS_INC_UART_PROTOCOL_INSIDE_H_
#define __LIBUTILS_INC_UART_PROTOCOL_INSIDE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBUTILS_INC_UART_PROTOCOL_INSIDE_GB
#define LIBUTILS_INC_UART_PROTOCOL_INSIDE_EX extern
#else
#define LIBUTILS_INC_UART_PROTOCOL_INSIDE_EX
#endif

#include <sys/types.h>
#include "utils.h"

#define UART_PROTOCOL_BUF_LEN       (64)
typedef struct {
    char buf[UART_PROTOCOL_BUF_LEN];
    size_t len;
} buf_t;

// note: 在接收底层数据中，一包最多解除4个frame
#define UART_PROTOCOL_FRAME_NUM     (4)
typedef struct {
    char *frame[UART_PROTOCOL_FRAME_NUM];
    size_t len[UART_PROTOCOL_FRAME_NUM];
} frame_cnt_t;

typedef enum {
    CMD_TYPE_UP             = 0x10,
    CMD_TYPE_DOWN           = 0x30,
    CMD_TYPE_RETRANSMISSION = 0x50,
    CMD_TYPE_RESPOND        = 0x70,
} cmd_type_t;


LIBUTILS_INC_UART_PROTOCOL_INSIDE_EX void UartProtocolDumpHex(char *sign, char *buf, size_t len);
#ifdef __cplusplus
}
#endif

#endif //__LIBUTILS_INC_UART_PROTOCOL_INSIDE_H_

