/**
 *
 * Release under GPLv2.
 * 
 * @file    uart_wrapper.h
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
#ifndef __LIBUTILS_INC_UART_WRAPPER_H_
#define __LIBUTILS_INC_UART_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_NUM_1,
    UART_NUM_2,
    UART_NUM_MAX,
} UartNum_t;

typedef enum {
    UART_SPEED_4800,
    UART_SPEED_9600,
    UART_SPEED_115200,
    UART_SPEED_MAX,
} UartSpeed_t;

typedef enum {
    FLOW_CONTROL_NONE,
    FLOW_CONTROL_SOFT,
    FLOW_CONTROL_HARDWARE,
    FLOW_CONTROL_MAX,
} UartFlowControl_t;

typedef enum {
    DATA_BIT_5,
    DATA_BIT_6,
    DATA_BIT_7,
    DATA_BIT_8,
    DATA_BIT_MAX,
} UartDataBit_t;

typedef enum {
    PARITY_NONE,
    PARITY_ODD,
    PARITY_EVEN,
    PARITY_MAX,
} UartParityType_t;

typedef enum {
    STOP_BIT_1,
    STOP_BIT_2,
    STOP_BIT_MAX,
} UartStopBit_t;

typedef void (*UartRead_cb_t)(char *buf, size_t len);

/**
 * @brief 串口配置结构体
 *
 * @param num
 * @param speed 
 * @param flow_control
 * @param data_bit
 * @param parity_type
 * @param stop_bit
 * @param read_cb: 串口读回调函数 (注意在回调中不能做耗时操作)
 *
 */
typedef struct {
    UartNum_t           num;
    UartSpeed_t         speed;
    UartFlowControl_t   flow_control;
    UartDataBit_t       data_bit;
    UartParityType_t    parity_type;
    UartStopBit_t       stop_bit;

    UartRead_cb_t       read_cb;
} UartConfig_t;


/**
 * @brief 
 *
 * @param uart_config
 *
 * @return 
 */
void *UartInit(UartConfig_t *uart_config);
void UartFinal(void *handle);

int UartWrite(void *handle, void *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif // __LIBUTILS_INC_UART_WRAPPER_H_

