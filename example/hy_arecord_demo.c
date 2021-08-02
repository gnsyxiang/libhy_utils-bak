/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_arecord_demo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/07 2021 17:25
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/07 2021      create the file
 * 
 *     last modified: 30/07 2021 17:25
 */
#include <stdio.h>

#include "hy_alsa.h"

#include "hy_module.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

typedef struct {
    void *log_handle;
    void *alsa_handle;
} _main_context_t;

static void _module_destroy(_main_context_t **context_pp)
{
    _main_context_t *context = *context_pp;

    // note: 增加或删除要同步到module_create_t中
    module_destroy_t module[] = {
        {"alsa",    &context->alsa_handle,  HyAlsaDestroy},
        {"log",     &context->log_handle,   HyLogDestroy},
    };

    RUN_DESTROY(module);

    HY_FREE(context_pp);
}

static _main_context_t *_module_create(void)
{
    _main_context_t *context = (_main_context_t *)HY_MALLOC_RET_VAL(sizeof(*context), NULL);

    HyLogConfig_t log_config;
    log_config.buf_len      = 512;
    log_config.level        = HY_LOG_LEVEL_INFO;
    log_config.config_file  = "./res/config/log4cplus.rc";

    HyAlsaConfig_t alsa_config;
    alsa_config.type                        = HY_ALSA_OPT_RECORD;
    alsa_config.save_config.snd_dev_name    = "hw:1,0";
    alsa_config.save_config.format_name     = "S16_LE";
    alsa_config.save_config.channels        = 1;
    alsa_config.save_config.sample_rate     = 16000;
    alsa_config.save_config.period_time     = 100;

    // note: 增加或删除要同步到module_destroy_t中
    module_create_t module[] = {
        {"log",     &context->log_handle,   &log_config,    (create_t)HyLogCreate,      HyLogDestroy},
        {"alsa",    &context->alsa_handle,  &alsa_config,   (create_t)HyAlsaCreate,     HyAlsaDestroy},
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

    while (1) {
        sleep(1);
    }

    _module_destroy(&context);

    return 0;
}

