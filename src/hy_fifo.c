/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_fifo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    23/09 2020 16:02
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        23/09 2020      create the file
 * 
 *     last modified: 23/09 2020 16:02
 */
#include <stdio.h>
#include <string.h>

#include "hy_fifo.h"

#include "hy_log.h"

#ifdef USE_DEBUG
    #define ALONE_DEBUG 1
    #define LOG_CATEGORY_TAG "hy_fifo"
#endif

#define _print_hex_macro(buf, len)              \
    do {                                        \
        for (uint32_t i = 0; i < (len); i++) {  \
            printf("%02x ", (buf)[i]);          \
        }                                       \
    } while (0)

static inline void _fifo_print_all_buf(HyFifoContext_t *context)
{
    LOGD("len: %d \n", context->len);

    _print_hex_macro(context->buf, context->len);
    printf("\n");
}

static inline void _fifo_print_content(HyFifoContext_t *context)
{
    LOGD("cnt: %d, head: %d, tail: %d \n", context->cnt, context->head, context->tail);

    if (context->tail >= context->head) {
        if (context->tail + context->cnt <= context->len) {
            _print_hex_macro(&context->buf[context->tail], context->cnt);
        } else {
            uint32_t tmp_len = context->len- context->tail;
            _print_hex_macro(&context->buf[context->tail], tmp_len);
            _print_hex_macro(context->buf, context->cnt - tmp_len);
        }
    } else {
        _print_hex_macro(context->buf, context->cnt);
    }
    printf("\n");
}

void HyFifoDump(HyFifoContext_t *context)
{
    if (!context) {
        LOGE("the param is NULL \n");
        return ;
    }

    if (context->cnt <= 0) {
        return;
    }

    _fifo_print_all_buf(context);
    _fifo_print_content(context);
}

uint32_t HyFifoInsertData(HyFifoContext_t *context, char *buf, uint32_t len)
{
    if (!context) {
        LOGE("the param is NULL \n");
        return 0;
    }

    if (context->head + len <= context->len) {
        memcpy(&context->buf[context->head], buf, len);
    } else {
        uint32_t tmp_len = context->len - context->head;
        memcpy(&context->buf[context->head], buf, tmp_len);
        memcpy(context->buf, buf + tmp_len, len - tmp_len);
    }

    context->cnt    += len;
    context->head   += len;
    context->head   %= context->len;

    if (context->cnt > context->len && context->head > context->tail) {
        context->tail = context->head;
    }

    if (context->cnt > context->len) {
        context->cnt = context->len;
    }

    return len;
}

static uint32_t _fifo_get_data_com(HyFifoContext_t *context, char *buf, uint32_t len)
{
    if (len > context->cnt) {
        LOGI("the len is too big, len: %d, cnt: %d \n", len, context->cnt);
        len = context->cnt;
    }

    if (context->cnt == 0 && context->tail == context->head) {
        return 0;
    } else if (context->tail > context->head) {
        if (context->tail + len <= context->len) {
            memcpy(buf, &context->buf[context->tail], len);
        } else {
            uint32_t tmp_len = context->len - context->tail;
            memcpy(buf, &context->buf[context->tail], tmp_len);
            memcpy(buf + tmp_len, context->buf, len - tmp_len);
        }
    } else {
        memcpy(buf, &context->buf[context->tail], len);
    }

    return len;
}

uint32_t HyFifoGetData(HyFifoContext_t *context, char *buf, uint32_t len)
{
    if (!context) {
        LOGE("the param is NULL \n");
        return 0;
    }

    len = _fifo_get_data_com(context, buf, len);

    context->cnt    -= len;
    context->tail   += len;
    context->tail   %= context->len;

    if (context->cnt <= 0) {
        context->cnt = 0;
    }

    return len;
}

uint32_t HyFifoPeekData(HyFifoContext_t *context, char *buf, uint32_t len)
{
    if (!context) {
        LOGE("the param is NULL \n");
        return 0;
    }

    return _fifo_get_data_com(context, buf, len);
}

int32_t HyFifoGetInfo(HyFifoContext_t *context, HyFifoInfoType_t type)
{
    if (!context) {
        LOGE("the param is NULL \n");
        return -1;
    }

    uint32_t len = 0;

    switch (type) {
        case HY_FIFO_INFO_LEN_TOTAL:
            len = context->len;
            break;
        case HY_FIFO_INFO_LEN_USED:
            len = context->cnt;
            break;
        case HY_FIFO_INFO_LEN_FREE:
            len = context->len - context->cnt;
            break;
    }

    return len;
}

void HyFifoClean(HyFifoContext_t *context)
{
    if (!context) {
        LOGE("the param is NULL \n");
        return ;
    }

    memset(context->buf, '\0', context->len);
    context->cnt    = 0;
    context->head   = 0;
    context->tail   = 0;
}

void HyFifoCreate(HyFifoContext_t *context, char *buf, uint32_t len)
{
    if (!context || !buf || len <= 0) {
        LOGE("the param is error \n");
        return;
    }

    memset(context, '\0', HY_FIFO_CONTEXT_T_LEN);

    context->buf    = buf;
    context->len    = len;

    LOGD("fifo create successful \n");
}

void HyFifoDestroy(HyFifoContext_t *context)
{
    if (!context) {
        LOGE("the param is NULL \n");
        return;
    }

    memset(context, '\0', HY_FIFO_CONTEXT_T_LEN);

    LOGD("fifo destroy successful \n");
}

