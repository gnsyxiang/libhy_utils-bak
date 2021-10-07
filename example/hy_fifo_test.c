/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_fifo_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    23/09 2020 16:08
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        23/09 2020      create the file
 * 
 *     last modified: 23/09 2020 16:08
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hy_module.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_type.h"
#include "hy_utils.h"
#include "hy_fifo.h"
#include "hy_thread.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

typedef struct {
    void *log_handle;
    void *fifo_handle;
} _main_context_t;

static void _module_destroy(_main_context_t **context_pp)
{
    _main_context_t *context = *context_pp;

    // note: 增加或删除要同步到module_create_t中
    module_destroy_t module[] = {
        {"fifo",            &context->fifo_handle,  HyFifoDestroy},
        {"log",             &context->log_handle,   HyLogDestroy},
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

    HyFifoConfig_t fifo_config;
    fifo_config.save_config.size = 16;

    // note: 增加或删除要同步到module_destroy_t中
    module_create_t module[] = {
        {"log",             &context->log_handle,   &log_config,            (create_t)HyLogCreate,    HyLogDestroy},
        {"fifo",            &context->fifo_handle,  &fifo_config,           (create_t)HyFifoCreate,   HyFifoDestroy},
    };

    RUN_CREATE(module);

    return context;
}

int main(int argc, char *argv[])
{
    int32_t i;
    char c = 'a';
    _main_context_t *context = _module_create();
    if (!context) {
        LOGE("_module_create faild \n");
        return -1;
    }

    LOGI("version: %s, data: %s, time: %s \n", "0.1.0", __DATE__, __TIME__);

    for (i = 0; i < 17; ++i) {
        HyFifoPut(context->fifo_handle, &c, 1);
        c++;
    }

    HyFifoDump(context->fifo_handle);

    // while (1) {
        // sleep(1);
    // }

    _module_destroy(&context);

    return 0;
}
