/**
 *
 * Release under GPL-3.0.
 *
 * @file    uart_protocol.c
 * @brief
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/11 2019 10:07
 * @version v0.0.1
 *
 * @since    note
 * @note     note
 *
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/11 2019      create the file
 *
 *     last modified: 26/11 2019 10:07
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define LIBUTILS_INC_UART_PROTOCOL_GB
#include "uart_protocol.h"
#undef LIBUTILS_INC_UART_PROTOCOL_GB

#include "uart_wrapper.h"
#include "utils.h"

#define uart_protocol_log(fmt, ...) \
    printf("<%s:%d, result: %s> " fmt, \
           __func__, __LINE__, strerror(errno), ##__VA_ARGS__);

#define UART_PROTOCOL_BUF_LEN       (32)
#define FRAME_HEAD                  (0x3c)
#define FRAME_TAIL                  (0x3e)

typedef struct {
    unsigned short head;
} UNPACK head_t;

typedef enum {
    CMD_TYPE_UP             = 0x10,
    CMD_TYPE_DOWN           = 0x30,
    CMD_TYPE_RETRANSMISSION = 0x50,
    CMD_TYPE_RESPOND        = 0x70,
} cmd_type_t;

/**
 * @brief 命令说明
 * cmd_type: 表示命令的种类，详见cmd_type_t;
 * cmd_num: 表示具体的命令，详见UartProtocolCmdSetting_t;
 */
typedef struct {
    char cmd_type;
    char cmd_num;
} UNPACK cmd_t;

/**
 * @brief 长度说明
 *
 */
typedef struct {
    unsigned short byte_cnt;
} UNPACK bc_t;

/**
 * @brief 数据校验
 *
 */
typedef struct {
    unsigned short check_sum;
} UNPACK cksum_t;

/*
 * 协议说明：
 * 1, head:     帧头
 * 2, cmd:      命令说明，详细见cmd_t
 * 3, bc:       数据长度，详见bc_t
 */
typedef struct {
    head_t  head;
    cmd_t   cmd;
    bc_t    bc;
    cksum_t cksum;
    char    data[0];
} UNPACK frame_t;

static inline void _init_frame_head(frame_t *frame)
{
    frame->head.head = MK_SHORT(FRAME_HEAD, FRAME_HEAD);
}

static inline void 
_init_frame_cmd(frame_t *frame, cmd_type_t cmd_type, UartProtocolCmdSetting_t cmd)
{
    frame->cmd.cmd_type = cmd_type;
    frame->cmd.cmd_num  = cmd;
}

static inline void _init_frame_bc(frame_t *frame, unsigned short bc)
{
    frame->bc.byte_cnt = bc;
}

static unsigned short _init_frame_cksum(uint8_t *data, uint16_t len) {
    uint64_t cksum = 0;
    int i = 0;
    while (i < len) {
        if (i + 2 > len) {
            cksum += data[i];
        } else {
            cksum += data[i + 1] << 8 | data[i];
        }
        i += 2;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >>16);
    cksum = (uint16_t)~cksum;
    return ((cksum >> 8) | (cksum & 0xff) << 8);
    //FIXME 检验位还没有写
}

static inline void _init_frame_data(frame_t *frame, char *buf, int len)
{
    memcpy(frame->data, buf, len);
}

/**
 * @brief 
 */
typedef struct {
    UartProtocolConfig_t    config;
    int                     fd;
} uart_protocol_state_t;

void *UartProtocol_Init(UartProtocolConfig_t *config)
{
    if (!config) {
        uart_protocol_log("the config is NULL \n");
        return NULL;
    }

    uart_protocol_state_t *uart_protocol_state = calloc(1, DATA_TYPE_LEN(uart_protocol_state_t));
    if (!uart_protocol_state) {
        uart_protocol_log("calloc is faild \n");
        return NULL;
    }

    UartConfig_t uart_config;
    uart_config.num          = UART_NUM_0;
    uart_config.speed        = UART_SPEED_115200;
    uart_config.flow_control = FLOW_CONTROL_NONE;
    uart_config.data_bit     = DATA_BIT_8;
    uart_config.parity_type  = PARITY_NONE;
    uart_config.stop_bit     = STOP_BIT_1;

    uart_protocol_state->fd = UartInit(&uart_config);
    if (uart_protocol_state->fd < 0) {
        uart_protocol_log("calloc is faild \n");
        free(uart_protocol_state);
        return NULL;
    }

    // 结构体里面的指针不会有两次释放的操作，所以没有风险
    uart_protocol_state->config = *config;

    if (uart_protocol_state->config.interface_type == UART_PROTOCOL_INTERFACE_ASYNC) {
    }

    return (void *)uart_protocol_state;
}

int UartProtocol_Final(void *handle)
{
    if (!handle) {
        uart_protocol_state_t *uart_protocol_state = handle;
        if (uart_protocol_state->fd > 0) {
            close(uart_protocol_state->fd);
            uart_protocol_log("close uart fd \n");
        }
        free(uart_protocol_state);
    }
    return 0;
}

static int _cmd_num_setting_power(char *buf)
{
    return 0;
}

static int _cmd_num_setting_query(char *buf)
{
    return 0;
}

typedef int (*cmd_setting_cb_t)(char *buf);
typedef struct {
    UartProtocolCmdSetting_t    cmd;
    cmd_setting_cb_t            cmd_setting_cb;
} call_cmd_func_t;

static call_cmd_func_t g_call_cmd_func[UART_PROTOCOL_CMD_MAX] = {
    {UART_PROTOCOL_CMD_POWER,   _cmd_num_setting_power},
    {UART_PROTOCOL_CMD_QUERY,   _cmd_num_setting_query},
};

static int _write_frame(void *handle, UartProtocolCmdSetting_t cmd)
{
    char buf[UART_PROTOCOL_BUF_LEN] = {0};
    int len = g_call_cmd_func[cmd].cmd_setting_cb(buf);

    int bc  = DATA_TYPE_LEN(frame_t) + len;
    frame_t *frame = calloc(1, bc);
    if (!frame) {
        uart_protocol_log("calloc faild \n");
        return -1;
    }

    _init_frame_head(frame);
    _init_frame_cmd(frame, CMD_TYPE_DOWN, cmd);
    _init_frame_bc(frame, bc);
    _init_frame_data(frame, buf, len);
    _init_frame_cksum((uint8_t *)frame, bc);

    uart_protocol_state_t *uart_protocol_state = handle;
    return UartWrite(uart_protocol_state->fd, frame, bc);
}

#define _judge_param(handle)                            \
    if (!handle) {                                      \
        uart_protocol_log("the handle is NULL \n");     \
        return -1;                                      \
    }

int UartProtocol_WriteFrame(void *handle, UartProtocolCmdSetting_t cmd)
{
    _judge_param(handle);

    return _write_frame(handle, cmd);
}

int UartProtocol_ReadFrame(void *handle, char * const buf)
{
    _judge_param(handle);

    return 0;
}

