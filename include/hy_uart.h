/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_uart.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    14/09 2019 12:18
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        14/09 2019      create the file
 * 
 *     last modified: 14/09 2019 12:18
 */
#ifndef __LIBUTILS_INCLUDE_HY_UART_H_
#define __LIBUTILS_INCLUDE_HY_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// @FIXME: open串口为阻塞情况下，这两个参数适配应用层协议
#define UART_READ_VMIN_LEN      (0)
#define UART_READ_VTIME_100MS   (1)

typedef enum {
    HY_UART_NUM_0,
    HY_UART_NUM_1,

    HY_UART_NUM_MAX,
} HyUartNum_t;

typedef enum {
    HY_UART_SPEED_4800,
    HY_UART_SPEED_9600,
    HY_UART_SPEED_115200,

    HY_UART_SPEED_MAX,
} HyUartSpeed_t;

typedef enum {
    HY_UART_FLOW_CONTROL_NONE,
    HY_UART_FLOW_CONTROL_SOFT,
    HY_UART_FLOW_CONTROL_HARDWARE,

    HY_UART_FLOW_CONTROL_MAX,
} HyUartFlowControl_t;

typedef enum {
    HY_UART_DATA_BIT_5,
    HY_UART_DATA_BIT_6,
    HY_UART_DATA_BIT_7,
    HY_UART_DATA_BIT_8,

    HY_UART_DATA_BIT_MAX,
} HyUartDataBit_t;

typedef enum {
    HY_UART_PARITY_NONE,
    HY_UART_PARITY_ODD,
    HY_UART_PARITY_EVEN,

    HY_UART_PARITY_MAX,
} HyUartParityType_t;

typedef enum {
    HY_UART_STOP_BIT_1,
    HY_UART_STOP_BIT_2,

    HY_UART_STOP_BIT_MAX,
} HyUartStopBit_t;

typedef void (*HyUartReadCb_t)(void *buf, uint32_t len, void *args);
typedef struct {
    HyUartReadCb_t  read_cb;
    void            *args;
} HyUartHandleCb_t;

#define DEV_PATH_NAME_LEN (32)
typedef struct {
    char                    dev_path_name[DEV_PATH_NAME_LEN];
    HyUartSpeed_t           speed;
    HyUartFlowControl_t     flow_control;
    HyUartDataBit_t         data_bit;
    HyUartParityType_t      parity_type;
    HyUartStopBit_t         stop_bit;
} HyUartConfig_t;

void *HyUartCreate(HyUartConfig_t *uart_config, HyUartHandleCb_t *handle_cb);
void HyUartDestroy(void *handle);

uint32_t HyUartWrite(void *handle, const void *buf, uint32_t len);
uint32_t HyUartRead(void *handle, void *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif // __LIBUTILS_INC_UART_WRAPPER_H_

