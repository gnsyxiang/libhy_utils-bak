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

typedef struct {
    char        *buf;
    uint32_t    len;

    uint32_t    cnt;
    int32_t    head;
    int32_t    tail;
} fifo_context_t;
#define FIFO_CONTEXT_T_LEN  (sizeof(fifo_context_t))

static fifo_context_t fifo_context;

#define _print_hex_macro(buf, len)              \
    do {                                        \
        for (uint32_t i = 0; i < (len); i++) {  \
            printf("%02x ", (buf)[i]);          \
        }                                       \
    } while (0)

static inline void _fifo_print_all_buf(fifo_context_t *context)
{
    LOGD("len: %d \n", context->len);

    printf("\n");
    _print_hex_macro(context->buf, context->len);
    printf("\n\n");
}

static inline void _fifo_print_content(fifo_context_t *context)
{
    LOGD("cnt: %d, head: %d, tail: %d \n", context->cnt, context->head, context->tail);

    printf("\n");
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
    printf("\n\n");
}

void HyFifoDump(void *handle)
{
    if (!handle) {
        LOGE("the param is NULL \n");
        return ;
    }

    fifo_context_t *context = handle;

    _fifo_print_all_buf(context);
    _fifo_print_content(context);
}

uint32_t HyFifoInsertData(void *handle, char *buf, uint32_t len)
{
    if (!handle) {
        LOGE("the param is NULL \n");
        return 0;
    }

    fifo_context_t *context = handle;
    uint32_t tmp_len = context->len - context->head;

    if (context->head + len <= tmp_len) {
        memcpy(&context->buf[context->head], buf, len);
    } else {
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

static uint32_t _fifo_get_data_com(fifo_context_t *context, char *buf, uint32_t len)
{
    if (len > context->cnt) {
        LOGW("the len is too big, len: %d, cnt: %d \n", len, context->cnt);
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

uint32_t HyFifoGetData(void *handle, char *buf, uint32_t len)
{
    if (!handle) {
        LOGE("the param is NULL \n");
        return 0;
    }

    fifo_context_t *context = handle;
    len = _fifo_get_data_com(context, buf, len);

    context->cnt    -= len;
    context->tail   += len;
    context->tail   %= context->len;

    if (context->cnt <= 0) {
        context->cnt = 0;
    }

    return len;
}

uint32_t HyFifoPeekData(void *handle, char *buf, uint32_t len)
{
    if (!handle) {
        LOGE("the param is NULL \n");
        return 0;
    }

    return _fifo_get_data_com((fifo_context_t *)handle, buf, len);
}

int32_t HyFifoGetInfo(void *handle, HyFifoInfoType_t type)
{
    if (!handle) {
        LOGE("the param is NULL \n");
        return -1;
    }

    fifo_context_t *context = handle;
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
        default:
            LOGE("error case\n");
            break;
    }

    return len;
}

void *HyFifoCreate(char *buf, uint32_t len)
{
    if (!buf || len <= 0) {
        LOGE("the param is error \n");
        return NULL;
    }

    memset(&fifo_context, '\0', FIFO_CONTEXT_T_LEN);

    fifo_context.buf    = buf;
    fifo_context.len    = len;

    LOGD("fifo create successful \n");

    return (void *)&fifo_context;
}

void HyFifoDestroy(void *handle)
{
    memset(handle, '\0', FIFO_CONTEXT_T_LEN);

    LOGD("fifo destroy successful \n");
}

