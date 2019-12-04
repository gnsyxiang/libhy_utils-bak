/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    uart_protocol_v1.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    02/12 2019 16:34
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        02/12 2019      create the file
 * 
 *     last modified: 02/12 2019 16:34
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define LIBUTILS_INC_UART_PROTOCOL_V1_GB
#include "uart_protocol_v1.h"
#undef LIBUTILS_INC_UART_PROTOCOL_V1_GB

#define uart_protocol_v1_log(fmt, ...) \
    printf("<%s:%d, result: %s> " fmt, \
           __func__, __LINE__, strerror(errno), ##__VA_ARGS__);

// #define DECODE_DEBUG
#define UART_PROTOCOL_V1_BUF_LEN    (128)
#define FRAME_HEAD                  (0x3c)
#define FRAME_TAIL                  (0x3e)

typedef struct {
    uint16_t head;
} UNPACK head_t;

/**
 * @brief 长度说明
 *
 */
typedef struct {
    uint16_t byte_cnt;
} UNPACK bc_t;

/**
 * @brief 数据校验
 *
 */
typedef struct {
    uint16_t check_sum;
} UNPACK cksum_t;

/**
 * @brief 命令说明
 * cmd_type: 表示命令的种类，详见cmd_type_t;
 * cmd: 表示具体的命令，详见UartProtocolCmdSetting_t;
 */
typedef struct {
    char cmd_type;
    char cmd;
} UNPACK cmd_t;

/*
 * 协议说明：
 * 1, head:     帧头
 * 2, cmd:      命令说明，详细见cmd_t
 * 3, bc:       数据长度，详见bc_t
 * 4, data:     状态设置，详见uart_protocol_v1_state_bytes_t
 */
typedef struct {
    head_t  head;
    cksum_t cksum;
    cmd_t   cmd;
    bc_t    bc;
    char    data[0];
} UNPACK frame_t;

static inline void _init_frame_head(frame_t *frame)
{
    frame->head.head = MK_SHORT(FRAME_HEAD, FRAME_HEAD);
}

static inline int _check_frame_head(uint16_t *head)
{
    return (*head == MK_SHORT(FRAME_HEAD, FRAME_HEAD) ? 0 : -1);
}

static inline void _init_frame_cmd(frame_t *frame, cmd_t *cmd)
{
    frame->cmd.cmd_type = cmd->cmd_type;
    frame->cmd.cmd      = cmd->cmd;
}

static inline void _init_frame_bc(frame_t *frame, uint16_t bc)
{
    frame->bc.byte_cnt = bc;
}

static uint16_t _check_frame_cksum(frame_t *frame, uint16_t len) {
    char *data = (char *)frame;
    data += DATA_TYPE_LEN(head_t);
    data += DATA_TYPE_LEN(cksum_t);
    len  += DATA_TYPE_LEN(cmd_t);
    len  += DATA_TYPE_LEN(bc_t);
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
}

static inline void _init_frame_cksum(frame_t *frame, uint16_t len) {
    frame->cksum.check_sum = _check_frame_cksum(frame, len);
}

static inline void _init_frame_data(frame_t *frame, char *buf, int len)
{
    memcpy(frame->data, buf, len);
}

static void _dump_frame(frame_t *frame, int len)
{
    char *buf = (char *)frame;
    printf("%c%c ", buf[0], buf[1]); 

    for (int i = 2; i < len; i++) {
        printf("%02x ", (unsigned char)buf[i]);
    }
    printf("\n");
}

// 低比特在前，高比特在后
typedef struct {
    char none:7;
    char power:1;
} byte_1_t;

// note: frame_t中的data可以强制转化成下面的结构体
typedef struct {
    byte_1_t byte1;
} uart_protocol_v1_state_bytes_t;

typedef struct {
    uart_protocol_v1_state_bytes_t state;
} uart_protocol_v1_state_t;

int UartProtocolV1Init(void)
{
    return 0;
}

int UartProtocolV1Final(void)
{
    return 0;
}

int UartProtocolV1Encode(unsigned char **frame, buf_t *buf, UartProtocolCmd_t *cmd)
{
    size_t bc = DATA_TYPE_LEN(frame_t) + buf->len;
    *frame = calloc(1, bc);
    if (!*frame) {
        uart_protocol_v1_log("calloc is faild \n");
        return -1;
    }

    cmd_t cmd_set;
    cmd_set.cmd      = cmd->cmd;
    cmd_set.cmd_type = CMD_TYPE_DOWN;

    frame_t *frame_p = (frame_t *)*frame;

    _init_frame_head(frame_p);
    _init_frame_bc(frame_p, buf->len);
    _init_frame_cmd(frame_p, &cmd_set);
    _init_frame_data(frame_p, buf->buf, buf->len);
    _init_frame_cksum(frame_p, buf->len);

    _dump_frame(frame_p, bc);

    return bc;
}

static inline void _debug_decode(char *sign)
{
#ifdef DECODE_DEBUG
    uart_protocol_v1_log("-------------------------%s\n", sign);
#else
    (void)sign;
#endif
}

static inline void _debug_decode_data(char *sign, char *buf, size_t len)
{
#ifdef DECODE_DEBUG
    UartProtocolDumpHex(sign, buf, len);
#else
    (void)sign;
    (void)buf;
    (void)len;
#endif
}

int UartProtocolV1Decode(buf_t *buf, frame_cnt_t *frame_cnt)
{
    static int buf_save_len = 0;
    static char buf_save[UART_PROTOCOL_V1_BUF_LEN] = {0};
    int buf_save_offset = 0;
    int cnt = 0;

    _debug_decode_data("0", buf->buf, buf->len);
    memcpy(buf_save + buf_save_len, buf->buf, buf->len);
    buf_save_len += buf->len;

    while (buf_save_len > 0) {
        char *buf_save_plus_offset = buf_save + buf_save_offset;
        _debug_decode_data("1", buf_save_plus_offset, buf_save_len);
        if (buf_save_len <= DATA_TYPE_LEN(frame_t)) {
            _debug_decode("1");
            break;
        }

        _debug_decode_data("2", buf_save_plus_offset, buf_save_len);
        if (0 != _check_frame_head((uint16_t *)buf_save_plus_offset)) {
            buf_save_offset++;
            buf_save_len--;
            _debug_decode("2");
            continue;
        }

        _debug_decode_data("3", buf_save_plus_offset, buf_save_len);
        frame_t *frame_p = (frame_t *)buf_save_plus_offset;
        if (frame_p->bc.byte_cnt + DATA_TYPE_LEN(frame_t) > buf_save_len) {
            _debug_decode("3");
            break;
        }

        _debug_decode_data("4", buf_save_plus_offset, buf_save_len);
        if (frame_p->cksum.check_sum != _check_frame_cksum(frame_p, frame_p->bc.byte_cnt)) {
            buf_save_offset++;
            buf_save_len--;
            _debug_decode("4");
            continue;
        }

        _debug_decode_data("5", buf_save_plus_offset, buf_save_len);
        uint16_t bc = frame_p->bc.byte_cnt + DATA_TYPE_LEN(frame_t);
        frame_cnt->frame[cnt] = calloc(1, bc);
        if (!frame_cnt->frame[cnt]) {
            uart_protocol_v1_log("calloc faild \n");
            _debug_decode("5");
            break;
        }

        memcpy(frame_cnt->frame[cnt], frame_p, bc);
        frame_cnt->len[cnt] = bc;
        cnt++;

        memcpy(buf_save, buf_save + bc + buf_save_offset, bc + buf_save_offset);
        buf_save_len   -= bc;
        buf_save_offset = 0;

        _debug_decode_data("6", buf_save, buf_save_len);
    }
    return cnt;
}

// note: 同步所有状态到顶层protocol，根据具体协议解析 
int UartProtocolV1SyncState(char *buf, UartProtocolState_t * const state)
{
    frame_t *frame = (frame_t *)buf;
    (void)frame;

    state->power = 1;
    return 0;
}

// 既可以单独设置需要控制字节，也可以同步所有字节状态
static int _cmd_num_setting_power(buf_t *buf)
{
    for (int i = 0; i < 3; i++) {
        buf->buf[i] = i;
    }
    buf->len = 3;
    return 0;
}

static int _cmd_num_setting_query(buf_t *buf)
{
    (void)buf;
    return 0;
}

typedef int (*cmd_setting_cb_t)(buf_t *buf);
typedef struct {
    UartProtocolCmdSetting_t    cmd;
    cmd_setting_cb_t            cmd_setting_cb;
} call_cmd_func_t;

static call_cmd_func_t g_call_cmd_func[UART_PROTOCOL_CMD_MAX] = {
    {UART_PROTOCOL_CMD_POWER,   _cmd_num_setting_power},
    {UART_PROTOCOL_CMD_QUERY,   _cmd_num_setting_query},
};

int UartProtocolV1SetState(UartProtocolCmd_t *cmd, buf_t *buf)
{
    g_call_cmd_func[(int)cmd->cmd].cmd_setting_cb(buf);
    return 0;
}

