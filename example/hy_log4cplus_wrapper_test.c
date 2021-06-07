/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_log4cplus_wrapper_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    03/06 2021 10:08
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        03/06 2021      create the file
 * 
 *     last modified: 03/06 2021 10:08
 */
#include <stdio.h>

#include "hy_log.h"

#define ALONE_DEBUG 1

int main(int argc, char *argv[])
{
    HyLogCreate(HY_LOG_LEVEL_TRACE, 1024, "./res/config/log4cplus.rc");

    LOGF("----1\n");
    LOGE("----2\n");
    LOGW("----3\n");
    LOGI("----4\n");
    LOGD("----5\n");
    LOGT("----6\n");

    HyLogDestroy();

    return 0;
}

