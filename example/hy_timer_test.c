/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_timerwheel_rotation_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    16/07 2021 14:33
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        16/07 2021      create the file
 * 
 *     last modified: 16/07 2021 14:33
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "hy_timer.h"

#include "hy_module.h"
#include "hy_mem.h"
#include "hy_utils.h"
#include "hy_type.h"
#include "hy_log.h"

#define ALONE_DEBUG 1
// #define TEST_MEMORY_LEAK

typedef struct {
    void *log_handle;
} _main_context_t;

static void _module_destroy(_main_context_t **context_pp)
{
    _main_context_t *context = *context_pp;

    // note: 增加或删除要同步到module_create_t中
    module_destroy_t module[] = {
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

    // note: 增加或删除要同步到module_destroy_t中
    module_create_t module[] = {
        {"log",     &context->log_handle,   &log_config,    (create_t)HyLogCreate,      HyLogDestroy},
    };

    RUN_CREATE(module);

    return context;
}

static void _timer_cb(void *args)
{
    LOGD("------%p \n", args);
}

int main(int argc, char *argv[])
{
    _main_context_t *context = _module_create();
    if (!context) {
        LOGE("_module_create faild \n");
        return -1;
    }

    HyTimerServiceConfig_t timer_service_config;
    timer_service_config.save_config.slot_interval_ms   = 1;
    timer_service_config.save_config.slot_num           = 60;

    HyTimerCreate(&timer_service_config);

    HyTimerConfig_t timer_config;
    timer_config.expires        = 1;
    timer_config.repeat_flag    = 1;
    timer_config.timer_cb       = _timer_cb;
    timer_config.args           = context;

    HyTimerAdd(&timer_config);

#ifdef TEST_MEMORY_LEAK
    int cnt = 0;
    while (cnt++ < 8) {
        sleep(1);
    }
#else
    while (1) {
        sleep(1);
    }
#endif

    HyTimerDestroy();

    _module_destroy(&context);

    return 0;
}

