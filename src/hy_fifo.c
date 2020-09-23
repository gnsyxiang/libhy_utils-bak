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

void HyFifoDump(void *handle)
{
    if (!handle) {
        LOGE("the param is NULL \n");
        return ;
    }

    fifo_context_t *context = handle;

    LOGD("len: %d \n", context->len);
    printf("\n");
    for (uint32_t i = 0; i < context->len; i++) {
        printf("%02x ", context->buf[i]);
    }
    printf("\n\n");

    LOGD("cnt: %d \n", context->cnt);
    LOGD("head: %d \n", context->head);
    LOGD("tail: %d \n", context->tail);

    uint32_t i = 0;

    printf("\n");
    if (context->tail >= context->head) {
        if (context->tail + context->cnt <= context->len) {
            for (i = 0; i < context->cnt; i++) {
                printf("%02x ", context->buf[context->tail + i]);
            }
        } else {
            uint32_t tmp_len = context->len- context->tail;
            for (i = 0; i < tmp_len; i++) {
                printf("%02x ", context->buf[context->tail + i]);
            }
            for (i = 0; i < context->cnt - tmp_len; i++) {
                printf("%02x ", context->buf[i]);
            }
        }
    } else {
        for (i = 0; i < context->cnt; i++) {
            printf("%02x ", context->buf[i]);
        }
    }
    printf("\n\n");
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

uint32_t HyFifoGetData(void *handle, char *buf, uint32_t len)
{
    if (!handle) {
        LOGE("the param is NULL \n");
        return 0;
    }

    fifo_context_t *context = handle;
    if (len > context->cnt) {
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

    context->tail += len;
    context->tail %= context->len;

    context->cnt -= len;
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

    // fifo_context_t *context = handle;

    return 1;
}

void *HyFifoCreate(char *buf, uint32_t len)
{
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






