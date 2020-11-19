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
#include <stdlib.h>
#include <string.h>

#include "hy_fifo.h"

// #include "hy_utils.h"
#include "hy_log.h"

#ifdef USE_DEBUG
    #define ALONE_DEBUG 1
    #define LOG_CATEGORY_TAG "hy_fifo"
    // #define FIFO_PRINT_ALL
#endif

typedef struct {
    uint32_t    cnt;
    uint32_t    head;
    uint32_t    tail;

    uint32_t    len;
    char        *buf;
} fifo_context_t;
#define FIFO_CONTEXT_T_LEN (sizeof(fifo_context_t))

static fifo_context_t *_create_wrapper(uint32_t len)
{
    fifo_context_t *context = calloc(1, FIFO_CONTEXT_T_LEN);
    if (!context) {
        LOGE("calloc faild \n");
        return NULL;
    }

    context->buf = calloc(1, len);
    if (!context->buf) {
        LOGE("calloc faild \n");
        free(context);
        context = NULL;
        return NULL;
    }

    context->len = len;

    return context;
}

static void _destroy_wrapper(fifo_context_t *context)
{
    if (context->buf) {
        free(context->buf);
        context->buf = NULL;
    }

    if (context) {
        free(context);
        context = NULL;
    }
}

static inline void _print_hex_ascii(char *buf, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        if (buf[i] == 0x0d || buf[i] == 0x0a || buf[i] < 32 || buf[i] >= 127) {
            printf("%02x[ ]  ", (uint8_t)buf[i]);
        } else {
            printf("%02x[%c]  ", (uint8_t)buf[i], (uint8_t)buf[i]);
        }
    }
}

#ifdef FIFO_PRINT_ALL
static inline void _print_all_buf(fifo_context_t *context)
{
    LOGE("len: %d \n", context->len);

    _print_hex_ascii(context->buf, context->len);
    printf("\n");
}
#endif

static inline void _print_content(fifo_context_t *context)
{
    LOGE("cnt: %d, head: %d, tail: %d \n", context->cnt, context->head, context->tail);

    if (context->tail >= context->head) {
        if (context->tail + context->cnt <= context->len) {
            _print_hex_ascii(&context->buf[context->tail], context->cnt);
        } else {
            uint32_t tmp_len = context->len- context->tail;
            _print_hex_ascii(&context->buf[context->tail], tmp_len);
            _print_hex_ascii(context->buf, context->cnt - tmp_len);
        }
    } else {
        _print_hex_ascii(&context->buf[context->tail], context->cnt);
    }
    printf("\n");
}

static void _dump_wrapper(fifo_context_t *context)
{
    if (context->cnt <= 0) {
        LOGE("the len is zero\n");
        return;
    }

#ifdef FIFO_PRINT_ALL
    _print_all_buf(context);
#endif
    _print_content(context);
}

static inline void _clean_wrapper(fifo_context_t *context)
{
    memset(context->buf, '\0', context->len);

    context->cnt    = 0;
    context->head   = 0;
    context->tail   = 0;
}

static int32_t _get_info_wrapper(fifo_context_t *context, HyFifoInfoType_t type)
{
    uint32_t len = 0;

    switch (type) {
        case HY_FIFO_TOTAL_LEN:
            len = context->len;
            break;
        case HY_FIFO_USED_LEN:
            len = context->cnt;
            break;
        case HY_FIFO_FREE_LEN:
            len = context->len - context->cnt;
            break;
        default:
            LOGE("the type is ERROR, type: %d \n", type);
            break;
    }

    return len;
}

static uint32_t _insert_wrapper(fifo_context_t *context, const char *buf, uint32_t len)
{
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
        LOGE("the cnt is error, context_cnt: %d, context_len: %d \n", context->cnt, context->len);
        context->cnt = context->len;
    }

    return len;
}

static uint32_t _update_tail_wrapper(fifo_context_t *context, uint32_t len)
{
    if (len > context->cnt) {
        LOGE("the len is error, len: %d, fifo_cnt: %d \n", len, context->cnt);
        return 0;
    }

    context->cnt    -= len;
    context->tail   += len;
    context->tail   %= context->len;

    return len;
}

static uint32_t _get_data_com(fifo_context_t *context, char *buf, uint32_t len)
{
    if (len > context->cnt) {
        // LOGE("the len is too big, len: %d, cnt: %d \n", len, context->cnt);
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

static uint32_t _get_data_wrapper(fifo_context_t *context, const char *buf, uint32_t len)
{
    len = _get_data_com(context, (char *)buf, len);

    context->cnt    -= len;
    context->tail   += len;
    context->tail   %= context->len;

    return len;
}

static inline uint32_t _peek_data_wrapper(fifo_context_t *context, const char *buf, uint32_t len)
{
    return _get_data_com(context, (char *)buf, len);
}

//-------------------------------------------------------------

void *HyFifoCreate(uint32_t len)
{
    if (len <= 0) {
        LOGE("the param is NULL, len: %d \n", len);
        return NULL;
    }

    return _create_wrapper(len);
}

void HyFifoDestroy(void *handle)
{
    if (!handle) {
        LOGE("the param is NULL, handle: %p \n", handle);
        return;
    }

    _destroy_wrapper((fifo_context_t *)handle);
}

void HyFifoDump(void *handle)
{
    if (!handle) {
        LOGE("the param is NULL, handle: %p \n", handle);
        return;
    }

    _dump_wrapper((fifo_context_t *)handle);
}

void HyFifoClean(void *handle)
{
    if (!handle) {
        LOGE("the param is NULL, handle: %p \n", handle);
        return;
    }

    _clean_wrapper((fifo_context_t *)handle);
}

int32_t HyFifoGetInfo(void *handle, HyFifoInfoType_t type)
{
    if (!handle) {
        LOGE("the param is NULL, handle: %p \n", handle);
        return -1;
    }

    return _get_info_wrapper((fifo_context_t *)handle, type);
}

uint32_t HyFifoInsertData(void *handle, const char *buf, uint32_t len)
{
    if (!handle || !buf || 0 == len) {
        LOGE("the param is NULL, handle: %p, buf: %p, len: %d \n", handle, buf, len);
        return 0;
    }

    return _insert_wrapper((fifo_context_t *)handle, buf, len);
}

uint32_t HyFifoUpdateTail(void *handle, uint32_t len)
{
    if (!handle || 0 == len) {
        LOGE("the param is NULL, handle: %p, len: %d \n", handle, len);
        return 0;
    }

    return _update_tail_wrapper((fifo_context_t *)handle, len);
}

uint32_t HyFifoGetData(void *handle, const char *buf, uint32_t len)
{
    if (!handle || !buf || 0 == len) {
        LOGE("the param is NULL, handle: %p, buf: %p, len: %d \n", handle, buf, len);
        return 0;
    }

    return _get_data_wrapper((fifo_context_t *)handle, buf, len);
}

uint32_t HyFifoPeekData(void *handle, const char *buf, uint32_t len)
{
    if (!handle || !buf || 0 == len) {
        LOGE("the param is NULL, handle: %p, buf: %p, len: %d \n", handle, buf, len);
        return 0;
    }

    return _peek_data_wrapper((fifo_context_t *)handle, buf, len);
}

