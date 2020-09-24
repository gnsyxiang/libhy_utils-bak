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

#include "hy_log.h"
#include "hy_fifo.h"

#ifdef USE_DEBUG
    #define ALONE_DEBUG 1
    #define LOG_CATEGORY_TAG "hy_fifo_test"
#endif

#define LOG_BUF_LEN_MAX     (128)
#define TEST_FIFO_BUF_LEN   (6)

typedef struct {
    char                fifo_buf[TEST_FIFO_BUF_LEN];
    char                log_buf[LOG_BUF_LEN_MAX];
    HyFifoContext_t     fifo;
} main_context_t;

static main_context_t main_context;

int main(int argc, char const* argv[])
{
    HyLogInit(main_context.log_buf, LOG_BUF_LEN_MAX, LOG_LEVEL_INFO);

    HyFifoCreate(&main_context.fifo, main_context.fifo_buf, TEST_FIFO_BUF_LEN);
    uint32_t len = 0;

    len = HyFifoInsertData(&main_context.fifo, "12345678", strlen("12345678"));
    LOGD("len: %d \n", len);

    HyFifoDump(&main_context.fifo);

    char buf[TEST_FIFO_BUF_LEN] = {0};
    len = HyFifoGetData(&main_context.fifo, buf, 2);
    LOGD("len: %d \n", len);

    HyFifoDump(&main_context.fifo);

    len = HyFifoPeekData(&main_context.fifo, buf, 2);
    LOGD("len: %d \n", len);

    HyFifoDump(&main_context.fifo);

    HyFifoDestroy(&main_context.fifo);

    HyLogDestory();

    return 0;
}
