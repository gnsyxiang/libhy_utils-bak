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

/*
 * 协议说明：
 * 1, head:     帧头
 * 2, cmd:      命令说明，详细见cmd_t
 * 3, bc:       数据长度，详见bc_t
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

static inline int _check_frame_head(frame_t *frame)
{
    return (frame->head.head == MK_SHORT(FRAME_HEAD, FRAME_HEAD) ? 0 : -1);
}

static inline void _init_frame_cmd(frame_t *frame, cmd_t *cmd)
{
    frame->cmd.cmd_type = cmd->cmd_type;
    frame->cmd.cmd_num  = cmd->cmd_num;
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

int UartProtocolV1Init(void)
{
    return 0;
}

int UartProtocolV1Final(void)
{
    return 0;
}

int UartProtocolV1Encode(unsigned char **frame, buf_t *buf, cmd_t *cmd)
{
    size_t bc = DATA_TYPE_LEN(frame_t) + buf->len;
    *frame = calloc(1, bc);
    if (!*frame) {
        uart_protocol_v1_log("calloc is faild \n");
        return -1;
    }

    frame_t *frame_p = (frame_t *)*frame;

    _init_frame_head(frame_p);
    _init_frame_bc(frame_p, buf->len);
    _init_frame_cmd(frame_p, cmd);
    _init_frame_data(frame_p, buf->buf, buf->len);
    _init_frame_cksum(frame_p, buf->len);

    _dump_frame(frame_p, bc);

    return bc;
}

int UartProtocolV1Decode(buf_t *buf, char **frame)
{
    static int buf_save_len = 0;
    static int buf_save_offset = 0;
    static char buf_save[UART_PROTOCOL_V1_BUF_LEN] = {0};

    memcpy(buf_save + buf_save_len, buf->buf, buf->len);
    buf_save_len += buf->len;

    if (buf_save_len <= DATA_TYPE_LEN(frame_t)) {
        return -1;
    }

    frame_t *frame_p = (frame_t *)(buf_save + buf_save_offset);
    if (frame_p->bc.byte_cnt + DATA_TYPE_LEN(head_t) > buf_save_len) {
        return -1;
    }

    if (0 != _check_frame_head(frame_p)) {
        buf_save_offset++;
        return -1;
    }

    if (frame_p->cksum.check_sum != _check_frame_cksum(frame_p, frame_p->bc.byte_cnt)) {
        buf_save_offset++;
        return -1;
    }

    uint16_t bc = frame_p->bc.byte_cnt + DATA_TYPE_LEN(frame_t);
    *frame = calloc(1, bc);
    if (!*frame) {
        uart_protocol_v1_log("calloc faild \n");
        return -1;
    }

    memcpy(*frame, frame_p, bc);

    memcpy(buf_save, buf_save + bc, bc);
    buf_save_len   -= bc;
    buf_save_offset = 0;

    return bc;
}

int UartProtocolV1SyncState(char *frame, UartProtocolState_t * const state)
{
    (void)frame;
    (void)state;
    return 0;
}
