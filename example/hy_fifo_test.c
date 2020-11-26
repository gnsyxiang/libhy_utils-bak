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

#include "hy_fifo.h"
#include "hy_log.h"

#ifdef USE_DEBUG
    #define ALONE_DEBUG 1
    #define LOG_CATEGORY_TAG "hy_fifo_test"
#endif

#define TEST_FIFO_BUF_LEN   (6)

typedef struct {
    void        *fifo_handle;
} main_context_t;

static main_context_t main_context;

int main(int argc, char const* argv[])
{
    HyLogCreate(USE_LOG_LEVEL, 1024);

    main_context.fifo_handle = HyFifoCreate(TEST_FIFO_BUF_LEN);

    HyFifoInsertData(main_context.fifo_handle, "12345678", strlen("12345678"));
    HyFifoDump(main_context.fifo_handle);

    char buf[TEST_FIFO_BUF_LEN] = {0};
    HyFifoGetData(main_context.fifo_handle, buf, 2);
    HyFifoDump(main_context.fifo_handle);

    HyFifoPeekData(main_context.fifo_handle, buf, 2);
    HyFifoDump(main_context.fifo_handle);

    HyFifoDestroy(main_context.fifo_handle);

    HyLogDestory();

    return 0;
}
