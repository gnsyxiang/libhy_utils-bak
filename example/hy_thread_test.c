/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_thread_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    07/10 2021 09:47
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        07/10 2021      create the file
 * 
 *     last modified: 07/10 2021 09:47
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hy_module.h"
#include "hy_mem.h"
#include "hy_type.h"
#include "hy_utils.h"
#include "hy_thread.h"
#include "hy_string.h"
#include "hy_log.h"

#define ALONE_DEBUG 1
#define TEST_MEMORY_LEAK

typedef struct {
    void *log_handle;
    void *thread_handle;
} _main_context_t;

#ifdef TEST_MEMORY_LEAK
static int32_t exit_flag = 0;
#endif

static int32_t _print_loop_cb(void *args)
{
#ifdef TEST_MEMORY_LEAK
    while (!exit_flag) {
        LOGE("haha \n");
        sleep(1);
    }
    return -1;
#else
    LOGE("haha \n");
    sleep(1);
    return 0;
#endif
}

static void _module_destroy(_main_context_t **context_pp)
{
    _main_context_t *context = *context_pp;

    // note: 增加或删除要同步到module_create_t中
    module_destroy_t module[] = {
        {"thread",  &context->thread_handle,    HyThreadDestroy},
        {"log",     &context->log_handle,       HyLogDestroy},
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

    HyThreadConfig_t thread_config;
    thread_config.save_config.thread_loop_cb    = _print_loop_cb;
    thread_config.save_config.args              = context;
    HY_STRNCPY(thread_config.save_config.name, "print", HY_STRLEN("print"));

    // note: 增加或删除要同步到module_destroy_t中
    module_create_t module[] = {
        {"log",     &context->log_handle,       &log_config,        (create_t)HyLogCreate,      HyLogDestroy},
        {"thread",  &context->thread_handle,    &thread_config,     (create_t)HyThreadCreate,   HyThreadDestroy},
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

    LOGI("version: %s, data: %s, time: %s \n", "0.1.0", __DATE__, __TIME__);

#ifdef TEST_MEMORY_LEAK
    int32_t cnt = 0;
    while (cnt++ < 5) {
        sleep(1);
    }
    exit_flag = 1;
    sleep(2);
#else
    while (1) {
        sleep(1);
    }
#endif

    _module_destroy(&context);

    return 0;
}

