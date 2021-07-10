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
#include <stdlib.h>
#include <string.h>

#include "hy_log.h"
#include "hy_module.h"

#define ALONE_DEBUG 1
#define LOG_CATEGORY_TAG "main"

typedef struct {
    void *log_handle;
} _main_context_t;

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

static void _module_destroy(_main_context_t *context)
{
    // note: 增加或删除要同步到module_create_t中
    module_destroy_t module[] = {
        {"log",     context->log_handle,    HyLogDestroy},
    };

    RUN_DESTROY(module);

    free(context);
}

static _main_context_t *_module_create(void)
{
    _main_context_t *context = malloc(sizeof(*context));
    if (!context) {
        LOGE("malloc faild \n");
        return NULL;
    }
    memset(context, '\0', sizeof(*context));

    HyLogConfig_t log_config;
    log_config.buf_len = 512;
    log_config.level = HY_LOG_LEVEL_INFO;
    log_config.config_file = "./res/config/log4cplus.rc";

    // note: 增加或删除要同步到module_destroy_t中
    module_create_t module[] = {
        {"log",  &context->log_handle,   &log_config,    (create_t)HyLogCreate,    HyLogDestroy},
    };

    RUN_CREATE(module);

    return context;
}

int main(int argc, char *argv[])
{
    _main_context_t *context = _module_create();
    if (!context) {
        LOGE("_module_create faild \n");
        return -1;
    }

    _test_log();
    _test_print_hex();

    _module_destroy(context);

    return 0;
}

