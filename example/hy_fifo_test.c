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

int main(int argc, char const* argv[])
{
    char log_buf[LOG_BUF_LEN_MAX] = {0};
    HyLogInit(log_buf, LOG_BUF_LEN_MAX, LOG_LEVEL_INFO);

    char fifo_buf[TEST_FIFO_BUF_LEN] = {0};
    void *fifo_handle = HyFifoCreate(fifo_buf, TEST_FIFO_BUF_LEN);
    uint32_t len = 0;

    len = HyFifoInsertData(fifo_handle, "12345678", strlen("12345678"));
    LOGD("len: %d \n", len);

    HyFifoDump(fifo_handle);

    char buf[TEST_FIFO_BUF_LEN] = {0};
    len = HyFifoGetData(fifo_handle, buf, 2);
    LOGD("len: %d \n", len);

    HyFifoDump(fifo_handle);

    len = HyFifoPeekData(fifo_handle, buf, 2);
    LOGD("len: %d \n", len);

    HyFifoDump(fifo_handle);

    HyFifoDestroy(fifo_handle);

    HyLogDestory();

    return 0;
}
