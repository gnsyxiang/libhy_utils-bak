/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_log_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    18/03 2021 20:22
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        18/03 2021      create the file
 * 
 *     last modified: 18/03 2021 20:22
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hy_log.h"
#include "hy_module.h"
#include "hy_utils.h"
#include "hy_type.h"
#include "hy_mem.h"

#define ALONE_DEBUG 1

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

static void _test_log_hex(void)
{
    char *str = "hello world! hello world! hello world! hello world!";

    LOG_HEX_ASCII(str, strlen(str));

    LOG_HEX(str, strlen(str));
}

static void _module_destroy(_main_context_t **context_pp)
{
    _main_context_t *context = *context_pp;

    // note: 增加或删除要同步到module_create_t中
    module_destroy_t module[] = {
        {"log",     &context->log_handle,   HyLogDestroy},
    };

    RUN_DESTROY(module);

    HY_FREE_PP(context_pp);
}

static _main_context_t *_module_create(void)
{
    _main_context_t *context = HY_MALLOC_RET_VAL(_main_context_t *, sizeof(*context), NULL);

    HyLogConfig_t log_config;
    log_config.save_config.buf_len      = 512;
    log_config.save_config.level        = HY_LOG_LEVEL_TRACE;
    log_config.save_config.color_output = HY_FLAG_ENABLE;

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
    _test_log_hex();

    _module_destroy(&context);

    return 0;
}

