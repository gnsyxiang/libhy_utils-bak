/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_fifo.c
 * @brief   封装fifo操作函数
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
#include <assert.h>

#include "hy_fifo.h"

#include "hy_utils.h"
#include "hy_log.h"

#include "hy_hal/hy_type.h"
#include "hy_hal/hy_mem.h"

#ifndef NDEBUG
#   define ALONE_DEBUG 1
#   define LOG_CATEGORY_TAG "hy_fifo"
// #   define FIFO_PRINT_ALL
#endif

typedef struct {
    char        *buf;
    uint32_t    size;

    uint32_t    in;
    uint32_t    out;
} fifo_context_t;

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
    LOGE("len: %d \n", context->size);
    _print_hex_ascii(context->buf, context->size);
    printf("\n");
}
#endif

static inline void _print_content(fifo_context_t *context)
{
    uint32_t fifo_len = context->in - context->out;

    LOGE("cnt: %d, in: %d, out: %d \n", fifo_len, context->in, context->out);

    #define out_index (context->out & (context->size - 1))
    uint32_t len_tmp = HyUtilsMinMacro(fifo_len, context->size- out_index);
    _print_hex_ascii(context->buf + out_index, len_tmp);
    _print_hex_ascii(context->buf, fifo_len - len_tmp);
    printf("\n");
}

static uint32_t _get_data_com(fifo_context_t *context, const char *buf, uint32_t len)
{
#if 0
    /**
     * @brief 保存tail副本
     *
     * @note: 1, 避免下面宏两次读取tail变量，造成多线程引起的错误
     *        2，如果使用的是函数，就不需要保存tail副本，函数此时最好加上inline提高效率
     *        3，即使tail变量被其他线程更新也不会出现程序错误，最多就是少处理一点点数据，但数据还在fifo中
     */
    uint32_t head_tmp = context->head;

    // 比较获取的数据长度和实际fifo中存在的数据长度
    len = HyUtilsMinMacro(len, head_tmp - context->tail);
#endif

    len = HyUtilsMinMacro(len, context->in - context->out);

    #define out_index (context->out & (context->size - 1))
    uint32_t len_tmp = HyUtilsMinMacro(len, context->size - out_index);

    memcpy((void *)buf, context->buf + out_index, len_tmp);
    memcpy((void *)(buf + len_tmp), context->buf, len - len_tmp);

    return len;
}

//---------------------------------------------------------------------------

int HyFifoPut(void *handle, void *buf, size_t len)
{
    assert(handle || buf || len);
    // if (!handle || !buf || 0 == len) {
        // LOGE("the param is NULL, handle: %p, buf: %p, len: %d \n", handle, buf, len);
        // return 0;
    // }
    fifo_context_t *context = handle;

#if 0
    /**
     * @brief 保存tail副本
     *
     * @note: 1, 避免下面宏两次读取tail变量，造成多线程引起的错误
     *        2，如果使用的是函数，就不需要保存tail副本，函数此时最好加上inline提高效率
     *        3，即使tail变量被其他线程更新也不会出现程序错误，最多就是少处理一点点数据，但数据还在fifo中
     */
    uint32_t tail_tmp = context->tail;

    // 比较存取的数据长度和实际fifo中可被存入的数据长度
    len = HyUtilsMinMacro(len, context->len - context->head + tail_tmp);
#endif
    len = HyUtilsMinMacro(len, context->size - context->in + context->out);

    #define in_index (context->in & (context->size - 1))
    uint32_t len_tmp = HyUtilsMinMacro(len, context->size - in_index);

    memcpy(context->buf + in_index, buf, len_tmp);
    memcpy(context->buf, buf + len_tmp, len - len_tmp);
    context->in += len;

    return len;
}

uint32_t HyFifoGet(void *handle, void *buf, size_t len)
{
    if (!handle || !buf || 0 == len) {
        LOGE("the param is NULL, handle: %p, buf: %p, len: %d \n", handle, buf, len);
        return 0;
    }
    fifo_context_t *context = handle;
    len = _get_data_com(context, buf, len);
    context->out += len;

    return len;
}

uint32_t HyFifoPeek(void *handle, void *buf, size_t len)
{
    if (!handle || !buf || 0 == len) {
        LOGE("the param is NULL, handle: %p, buf: %p, len: %d \n", handle, buf, len);
        return 0;
    }
    fifo_context_t *context = handle;
    len = _get_data_com(context, buf, len);

    return len;
}

uint32_t HyFifoUpdateOut(void *handle, uint32_t len)
{
    if (!handle || 0 == len) {
        LOGE("the param is NULL, handle: %p, len: %d \n", handle, len);
        return 0;
    }
    fifo_context_t *context = handle;

    char buf[1024] = {0};
    len = _get_data_com(context, buf, len);
    context->out += len;

    return len;
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
        case HY_FIFO_TOTAL_LEN:
            len = context->size;
            break;
        case HY_FIFO_USED_LEN:
            len = context->in - context->out;
            break;
        case HY_FIFO_FREE_LEN:
            len = context->size - (context->in + context->out);
            break;
        default:
            LOGE("the type is ERROR, type: %d \n", type);
            break;
    }

    return len;
}

void HyFifoDump(void *handle)
{
    if (!handle) {
        LOGE("the param is NULL, handle: %p \n", handle);
        return;
    }
    fifo_context_t *context = handle;

#ifdef FIFO_PRINT_ALL
    _print_all_buf(context);
#endif
    _print_content(context);
}

void HyFifoClean(void *handle)
{
    JUDGE_NULL(!handle);

    fifo_context_t *context = handle;

    memset(context->buf, '\0', context->size);
    context->in = context->out = 0;
}

void HyFifoDestroy(void **handle)
{
    JUDGE_NULL(!handle || !*handle);

    fifo_context_t *context = *handle;

    if (context) {
        if (context->buf) {
            HY_FREE(&context->buf);
        }

        HY_FREE(handle);
    }
}

void *HyFifoCreate(uint32_t size)
{
    if (size <= 0) {
        LOGE("the param is NULL, len: %d \n", size);
        return NULL;
    }

    if (!HyUtilsIsPowerOf2(size) || size > 0x80000000) {
        size = HyUtilsNumTo2N2(size);
        LOGE("size must be power of 2, new size: %d \n", size);
    }

    fifo_context_t *context = calloc(1, sizeof(*context));
    if (!context) {
        LOGE("calloc faild \n");
        return NULL;
    }

    context->buf = calloc(1, size);
    if (!context->buf) {
        LOGE("calloc faild \n");

        HY_FREE(&context);
        return NULL;
    }
    context->size = size;

    return context;
}

