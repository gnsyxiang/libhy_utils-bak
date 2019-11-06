/**
 * 
 * Release under GPLv2.
 * 
 * @file    thread_wrapper_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    13/09 2019 21:40
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        13/09 2019      create the file
 * 
 *     last modified: 13/09 2019 21:40
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "thread_wrapper.h"
#include "signal_wrapper.h"
#include "utils.h"

int cnt = 6;
static void *_test_thread_loop(void *args)
{
    while (cnt != 1) {
        sleep(1);
        printf("hello test \n");
    }
    return NULL;
}

int main(int argc, const char *argv[])
{
    SignalHandleInit(argv[0]);

    ThreadParam_t thread_param;
    memset(&thread_param, '\0', DATA_TYPE_LEN(thread_param));
    thread_param.thread_loop = _test_thread_loop;

    Thread_CreateDetachedThread(&thread_param);

    while (cnt-- >= 0) {
        sleep(1);
        printf("hello main \n");
    }

    SignalHandleFinal();

    return 0;
}
