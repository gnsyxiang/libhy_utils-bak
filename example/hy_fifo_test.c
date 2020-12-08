/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_fifo_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    23/09 2020 16:08
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        23/09 2020      create the file
 * 
 *     last modified: 23/09 2020 16:08
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "hy_time.h"
#include "hy_pthread.h"
#include "hy_fifo.h"
#include "hy_log.h"

#ifdef USE_DEBUG
    #define ALONE_DEBUG 1
    #define LOG_CATEGORY_TAG "hy_fifo_test"
#endif

#define TEST_FIFO_BUF_LEN   (8)

typedef struct {
    void        *fifo_handle;
    void        *write_thread_handle;
    void        *read_thread_handle;
    uint32_t    cnt;
} main_context_t;

static main_context_t main_context;

static void _put_data_cb(void *args)
{
    main_context_t *context = args;

    static char i = 0;
    // LOGD("i: %d \n", i);

    HyTimeDelayUs(50);

    HyFifoPut(context->fifo_handle, &i, 1);
    HyFifoDump(context->fifo_handle);

    i++;
}

static void _get_data_cb(void *args)
{
    main_context_t *context = args;

    char buf[TEST_FIFO_BUF_LEN] = {0};
    // HyFifoPeekData(context->fifo_handle, buf, TEST_FIFO_BUF_LEN);
    // HyFifoDump(context->fifo_handle);

    uint32_t len = HyFifoGet(context->fifo_handle, buf, TEST_FIFO_BUF_LEN);
    LOGD("len: %d \n", len);

    HyTimeDelayUs(60);
    HyFifoDump(context->fifo_handle);

    PRINT_HEX_ASCII(buf, len);
}

int main(int argc, char const* argv[])
{
    HyLogCreate(USE_LOG_LEVEL, 1024);

    main_context_t *context = &main_context;
    memset(context, '\0', sizeof(*context));

    context->fifo_handle = HyFifoCreate(TEST_FIFO_BUF_LEN);
    if (!context->fifo_handle) {
        LOGE("hy_fifo create faild \n");
        return -1;
    }

    HyPthreadHandleCb_t handle_cb;
    handle_cb.loop_cb   = _put_data_cb;
    handle_cb.args      = context;
    context->write_thread_handle = HyPthreadCreate(&handle_cb);
    if (!context->write_thread_handle) {
        LOGE("hy_thread crteate faild \n");
        return -1;
    }

    handle_cb.loop_cb   = _get_data_cb;
    context->read_thread_handle = HyPthreadCreate(&handle_cb);
    if (!context->read_thread_handle) {
        LOGE("hy_thread crteate faild \n");
        return -1;
    }

    while (context->cnt < 2) {
        sleep(1);
        // HyTimeDelayUs(50);
        context->cnt++;
    }

    HyPthreadDestroy(context->write_thread_handle);
    HyPthreadDestroy(context->read_thread_handle);

    HyFifoDestroy(context->fifo_handle);
    HyLogDestory();

    return 0;
}
