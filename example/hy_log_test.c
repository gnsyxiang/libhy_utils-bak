/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_log_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/05 2021 08:19
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/05 2021      create the file
 * 
 *     last modified: 08/05 2021 08:19
 */
#include <stdio.h>
#include <string.h>

#include "hy_log.h"

#define ALONE_DEBUG 1
#define LOG_CATEGORY_TAG "main"

static void _test_log(void)
{
    LOGE("-1-hello world\n");
    LOGW("-2-hello world\n");
    LOGD("-3-hello world\n");
    LOGI("-4-hello world\n");
}

static void _test_print_hex(void)
{
    char *str = "hello world! hello world! hello world! hello world!";

    PRINT_HEX_ASCII(str, strlen(str));

    PRINT_HEX(str, strlen(str));
}

int main(int argc, char *argv[])
{
    HyLogConfig_t log_config;
    log_config.buf_len = 512;
    log_config.level = HY_LOG_LEVEL_INFO;
    log_config.config_file = "./res/config/log4cplus.rc";
    HyLogCreate(&log_config);

    _test_log();
    _test_print_hex();

    HyLogDestroy(NULL);

    return 0;
}

