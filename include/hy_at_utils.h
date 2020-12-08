/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_at_utils.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    23/10 2020 16:56
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        23/10 2020      create the file
 * 
 *     last modified: 23/10 2020 16:56
 */
#ifndef __LIBUTILS_INCLUDE_HY_AT_UTILS_H_
#define __LIBUTILS_INCLUDE_HY_AT_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "hy_list.h"
#include "hy_utils.h"

enum {
    AT_STATE_IDLE,
    AT_STATE_READ,
    AT_STATE_READ_HEAD,
    AT_STATE_READ_READY,
    AT_STATE_READ_READY_1,
    AT_STATE_READ_READY_2,
    AT_STATE_READ_OK,
    AT_STATE_READ_SIGNAL_CWJAP_DEF,
    AT_STATE_READ_IPD_DATA,
    AT_STATE_READ_FAILD,
    AT_STATE_READ_ERROR,
    AT_STATE_READ_ERROR_ALREADY_CONNECTED,
    AT_STATE_READ_BUSY_S,
    AT_STATE_READ_SEND,
    AT_STATE_READ_CLOSED,
    AT_STATE_READ_MAX,
    AT_STATE_WRITE,
    AT_STATE_WRITE_CMD,
    AT_STATE_WRITE_CMD_WAIT,
    AT_STATE_WRITE_DATA,
    AT_STATE_WRITE_MAX,
};

typedef struct {
    uint8_t     state;
    char        *recv_str;
} AtCatchStr_t;

typedef struct {
    AtCatchStr_t    *catch_str;
    uint8_t         catch_str_cnt;
} AtCatchStrParam_t; 

typedef void (*AtUtilsWriteCb_t)(BufUnion_t *buf_union, void *args);
typedef void (*AtUtilsHandleFrameCb_t)(int8_t type, BufUnion_t **buf_union, void *args);
typedef struct {
    AtUtilsWriteCb_t        write_cb;
    AtUtilsHandleFrameCb_t  handle_frame_cb;
    void                    *args;
} AtHandleCb_t;

void *HyAtUtilsCreate(AtHandleCb_t *handle_cb, uint32_t read_fifo_len);
void HyAtUtilsDestroy(void *handle);
void HyAtUtilsClean(void *handle);

typedef struct {
    char        *cmd;
    char        *data;
    uint16_t    cmd_len;
    uint16_t    data_len;
    uint16_t    wait_time;
    uint8_t     retry;
} AtUtilsCmd_t;

uint32_t HyAtUtilsWriteCmd(void *handle, AtUtilsCmd_t *cmd);
uint32_t HyAtUtilsRecvData(void *handle, BufUnion_t *buf_union);

uint8_t HyAtUtilsParseData(void *handle, AtCatchStrParam_t *catch_str_param);

#ifdef __cplusplus
}
#endif

#endif

