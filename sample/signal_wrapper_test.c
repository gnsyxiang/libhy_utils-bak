/**
 * 
 * Release under GPLv2.
 * 
 * @file    signal_wrapper_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    13/09 2019 11:20
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        13/09 2019      create the file
 * 
 *     last modified: 13/09 2019 11:20
 */
#include <stdio.h>
#include <unistd.h>

#include "signal_wrapper.h"

int main(int argc, const char *argv[])
{
    SignalHandleInit(argv[0]);

    while (1) {
        sleep(1);
    }

    SignalHandleFinal();

    return 0;
}

