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

static void _test_thread_loop(void *args)
{
    char *test = args;

    printf("--test--->> %s \n", test);

    int cnt = 3;
    while (cnt-- > 0) {
        sleep(1);
        printf("hello test \n");
    }
}

static char *test = "hello world";

int main(int argc, const char *argv[])
{
    SignalHandleInit(argv[0]);

    ThreadParam_t thread_param;
    memset(&thread_param, '\0', DATA_TYPE_LEN(thread_param));
    thread_param.thread_loop = _test_thread_loop;
    thread_param.name        = "test";
    thread_param.args        = test;

    Thread_CreateDetachedThread(&thread_param);

    char name[16] = {};
    int cnt = 5;
    while (cnt-- > 0) {
    // while (cnt-- >= 0) {
        sleep(1);
        printf("hello main \n");
        memset(name, '\0', sizeof(name));
        Thread_GetName(name);
        printf("-----------name: %s \n", name);
    }

    SignalHandleFinal();

    return 0;
}
