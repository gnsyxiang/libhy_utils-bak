/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    data_buf_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/11 2019 15:22
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/11 2019      create the file
 * 
 *     last modified: 05/11 2019 15:22
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"
#include "data_buf.h"
#include "thread_wrapper.h"
#include "signal_wrapper.h"
#include "core_dump.h"

#define READ_BUF_LEN (3)

static void _read_databuf_common(void *handle, int peek_flag)
{
    int ret;
    char read_buf[READ_BUF_LEN] = {0};
    if (peek_flag == 1) {
        ret = DataBufPeekRead(handle, read_buf, READ_BUF_LEN);
    } else {
        ret = DataBufRead(handle, read_buf, READ_BUF_LEN);
    }

    printf("read %d:  --->> ", ret);
    DumpHexData(read_buf, ret);
}

#define _read_databuf(handle)      _read_databuf_common(handle, 0)
#define _peek_read_databuf(handle) _read_databuf_common(handle, 1)

static void *_read_databuf_loop(void *args)
{
    while (1) {
        sleep(1);
#if 0
        _read_databuf(args);
#else
        _peek_read_databuf(args);
        // char buf[READ_BUF_LEN] = {0};
        // DataBufRemoveData(args, buf, READ_BUF_LEN);
#endif
    }
    return NULL;
}

static void _write_databuf(void *handle)
{
    char *buf = "123456789";
    int len = strlen(buf);
    int ret = DataBufWrite(handle, buf, len);

    printf("write %d:  ============>> ", ret);
    DumpHexData(buf, len);
}

static void *_write_databuf_loop(void *args)
{
    while (1) {
        _write_databuf(args);
        sleep(1);
    }
    return NULL;
}

int main(int argc, const char *argv[])
{
#define TEST_DATA_BUF_SIZE  (32)
#define THREAD_TEST_CNT     (10)

    SignalHandleInit(argv[0]);
    Core_OpenDump();

    DataBufConfig_t databuf_config;
    databuf_config.m_ability = DATABUF_ABILITY_THREAD_SAFETY;
    databuf_config.m_size    = TEST_DATA_BUF_SIZE;

    void *handle = DataBufInit(&databuf_config);

    ThreadParam_t thread_param;
    memset(&thread_param, '\0', DATA_TYPE_LEN(thread_param));
    thread_param.thread_loop = _write_databuf_loop;
    thread_param.args        = handle;

    for (int i = 0; i < THREAD_TEST_CNT; i++) {
        Thread_CreateDetachedThread(&thread_param);
    }

    thread_param.thread_loop = _read_databuf_loop;

    for (int i = 0; i < THREAD_TEST_CNT; i++) {
        Thread_CreateDetachedThread(&thread_param);
    }

    while (1) {
        sleep(1);
        DataBufDump(handle);
    }

    DataBufFinal(handle);
    SignalHandleFinal();
    
    return 0;
}

