/**
 *
 * Release under GPL-3.0.
 * 
 * @file    uart_protocol_v1.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    02/12 2019 16:35
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        02/12 2019      create the file
 * 
 *     last modified: 02/12 2019 16:35
 */
#ifndef __LIBUTILS_INC_UART_PROTOCOL_V1_H_
#define __LIBUTILS_INC_UART_PROTOCOL_V1_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBUTILS_INC_UART_PROTOCOL_V1_GB
#define LIBUTILS_INC_UART_PROTOCOL_V1_EX extern
#else
#define LIBUTILS_INC_UART_PROTOCOL_V1_EX
#endif

#include "uart_protocol.h"
#include "uart_protocol_inside.h"

int UartProtocolV1Init(void);
int UartProtocolV1Final(void);

int UartProtocolV1Encode(unsigned char **frame, buf_t *buf, cmd_t *cmd);
int UartProtocolV1Decode(buf_t *buf, frame_cnt_t *frame_cnt);

int UartProtocolV1SyncState(char *buf, UartProtocolState_t * const state);

#ifdef __cplusplus
}
#endif

#endif // __LIBUTILS_INC_UART_PROTOCOL_V1_H_

