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
#include <unistd.h>

#include "hy_timerwheel_rotation.h"

#include "hy_module.h"
#include "hy_mem.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

typedef struct {
    void *log_handle;
    void *timerwheel_totation_handle;
} _main_context_t;

static void _module_destroy(_main_context_t **context_pp)
{
    _main_context_t *context = *context_pp;

    // note: 增加或删除要同步到module_create_t中
    module_destroy_t module[] = {
        {"timerwheel_totation", &context->timerwheel_totation_handle,   HyTimerWheelRotationDestroy},
        {"log",                 &context->log_handle,                   HyLogDestroy},
    };

    RUN_DESTROY(module);

    HY_FREE(context_pp);
}

static _main_context_t *_module_create(void)
{
    _main_context_t *context = HY_MALLOC_RET_VAL(sizeof(*context), NULL);

    HyLogConfig_t log_config;
    log_config.buf_len      = 512;
    log_config.level        = HY_LOG_LEVEL_TRACE;
    log_config.config_file  = "./res/config/log4cplus.rc";

    HyTimerWheelRotationConfig_t timerwheel_rotation_config;
    timerwheel_rotation_config.slot_interval = 1;
    timerwheel_rotation_config.slot_num = 60;

    // note: 增加或删除要同步到module_destroy_t中
    module_create_t module[] = {
        {"log",                 &context->log_handle,                   &log_config,                    (create_t)HyLogCreate,                  HyLogDestroy},
        {"timerwheel_totation", &context->timerwheel_totation_handle,   &timerwheel_rotation_config,    (create_t)HyTimerWheelRotationCreate,   HyTimerWheelRotationDestroy},
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

    HyTimerHandleConfig_t timer_config;
    timer_config.args       = context;
    timer_config.expires    = 1;
    timer_config.timer_cb   = _timer_cb;

    for (int i = 0; i < 3; ++i) {
        timer_config.expires    += i;
        HyTimerWheelRotationAdd(context->timerwheel_totation_handle, &timer_config);
    }

    sleep(8);
    // while (1) {
        // sleep(1);
    // }

    _module_destroy(&context);

    return 0;
}

