/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_list_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/04 2020 16:12
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/04 2020      create the file
 * 
 *     last modified: 08/04 2020 16:12
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
#include "hy_list.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

typedef struct {
    char name[HY_STRING_BUF_MAX_LEN_32];
    int32_t id;

    struct hy_list_head list;
} _student_t;

typedef struct {
    void *log_handle;

    struct hy_list_head list;
} _main_context_t;

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

    LOGI("version: %s, data: %s, time: %s \n", "0.1.0", __DATE__, __TIME__);

    INIT_LIST_HEAD(&context->list);

    int32_t i;
    char buf[HY_STRING_BUF_MAX_LEN_32];
    #define STUDENT_CNT (5)
    _student_t student[STUDENT_CNT];
    for (i = 0; i < STUDENT_CNT; ++i) {
        HY_MEMSET(buf, HY_STRING_BUF_MAX_LEN_32);
        snprintf(buf, HY_STRING_BUF_MAX_LEN_32, "student%d", i);
        HY_STRNCPY(student[i].name, buf, HY_STRLEN(buf));
        student[i].id = i;

        list_add_tail(&student[i].list, &context->list);
    }

    _student_t *pos, *n;
    list_for_each_entry(pos, &context->list, list) {
        if (pos) {
            LOGI("name: %s, id: %d \n", pos->name, pos->id);
        }
    }

    list_for_each_entry_safe(pos, n, &context->list, list) {
        LOGI("name: %s, id: %d \n", pos->name, pos->id);

        list_del(&pos->list);
    }

    while (1) {
        sleep(1);
    }

    _module_destroy(&context);

    return 0;
}

