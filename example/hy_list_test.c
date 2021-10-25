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
#include "hy_signal.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

typedef struct {
    char name[HY_STRING_BUF_MAX_LEN_32];
    hy_s32_t id;

    struct hy_list_head list;
} _student_t;

typedef struct {
    void *log_handle;
    void *signal_handle;

    struct hy_list_head list;
    hy_u32_t list_cnt;

    hy_s32_t exit_flag;
} _main_context_t;

static struct hy_list_head *_get_node_ptr(struct hy_list_head *list, hy_s32_t index)
{
    struct hy_list_head *tmp = list;
    for (int i = 0; i <= index; ++i) {
        tmp = tmp->next;
    }
    return tmp;
}

static hy_u32_t _get_node_data(struct hy_list_head *list, hy_u32_t index)
{
    struct hy_list_head *tmp = _get_node_ptr(list, index);
    return (list_entry(tmp, _student_t, list))->id;
}

#if 0
static void _set_node_data(struct hy_list_head *list, hy_u32_t index, hy_s32_t id)
{
    struct hy_list_head *tmp = _get_node_ptr(list, index);
    (list_entry(tmp, _student_t, list))->id = id;
}
#endif

static void _swap_node_prt(struct hy_list_head *list, hy_s32_t a, hy_s32_t b)
{
    struct hy_list_head *tmp_a = _get_node_ptr(list, a);
    struct hy_list_head *tmp_b = _get_node_ptr(list, b);

    struct hy_list_head tmp = {NULL, NULL};
    list_add_tail(&tmp, tmp_a);
    list_del(tmp_a);
    list_add_tail(tmp_a, tmp_b);
    list_del(tmp_b);
    list_add_tail(tmp_b, &tmp);
    list_del(&tmp);
}

static hy_u32_t _get_partition(struct hy_list_head *list, hy_s32_t low, hy_s32_t high)
{
    hy_s32_t tmp_id = 0;
    hy_s32_t pivot_data = _get_node_data(list, low);

    while (low < high) {
        tmp_id = _get_node_data(list, high);
        while (low < high && tmp_id >= pivot_data) {
            high--;
            tmp_id = _get_node_data(list, high);
        }

        if (low != high) {
            _swap_node_prt(list, low, high);
        }

        tmp_id = _get_node_data(list, low);
        while (low < high && tmp_id <= pivot_data) {
            low++;
            tmp_id = _get_node_data(list, low);
        }

        if (low != high) {
            _swap_node_prt(list, high, low);
        }
    }

    return low;
}

static void _sort(struct hy_list_head *list, hy_s32_t low, hy_s32_t high)
{
    if (low < high) {
        hy_s32_t pivot_point = _get_partition(list, low, high);

        _sort(list, low, pivot_point - 1);
        _sort(list, pivot_point + 1, high);
    }
}

static void _dump_list(struct hy_list_head *list, const char *tag)
{
    LOGD("%s: \n", tag);

    _student_t *pos;
    list_for_each_entry(pos, list, list) {
        LOGI("name: %s, id: %d \n", pos->name, pos->id);
    }
}

static void _quick_sort(_main_context_t *context)
{
    _dump_list(&context->list, "before");
    _sort(&context->list, 0, context->list_cnt);
    _dump_list(&context->list, "after");
}

static void _signal_error_cb(void *args)
{
    LOGE("------error cb\n");

    _main_context_t *context = args;
    context->exit_flag = 1;
}

static void _signal_user_cb(void *args)
{
    LOGE("------user cb\n");

    _main_context_t *context = args;
    context->exit_flag = 1;
}

static void _module_destroy(_main_context_t **context_pp)
{
    _main_context_t *context = *context_pp;

    // note: 增加或删除要同步到module_create_t中
    module_destroy_t module[] = {
        {"signal",  &context->signal_handle,    HySignalDestroy},
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

    int8_t signal_error_num[HY_SIGNAL_NUM_MAX_32] = {
        SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGFPE,
        SIGSEGV, SIGBUS, SIGSYS, SIGXCPU, SIGXFSZ,
    };

    int8_t signal_user_num[HY_SIGNAL_NUM_MAX_32] = {
        SIGINT, SIGTERM, SIGUSR1, SIGUSR2,
    };

    HySignalConfig_t signal_config;
    memset(&signal_config, 0, sizeof(signal_config));
    HY_MEMCPY(signal_config.error_num, signal_error_num, sizeof(signal_error_num));
    HY_MEMCPY(signal_config.user_num, signal_user_num, sizeof(signal_user_num));
    signal_config.save_config.appname       = "template";
    signal_config.save_config.coredump_path = "./";
    signal_config.save_config.error_cb      = _signal_error_cb;
    signal_config.save_config.user_cb       = _signal_user_cb;
    signal_config.save_config.args          = context;

    // note: 增加或删除要同步到module_destroy_t中
    module_create_t module[] = {
        {"log",     &context->log_handle,       &log_config,        (create_t)HyLogCreate,      HyLogDestroy},
        {"signal",  &context->signal_handle,    &signal_config,     (create_t)HySignalCreate,   HySignalDestroy},
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

    // 初始化
    // INIT_LIST_HEAD
    // HY_LIST_HEAD

    // 曾
    // list_add
    // list_add_tail

    // 删
    // list_del

    // 改

    // 查
    // list_for_each
    // list_for_each_prev
    // list_for_each_safe
    // list_for_each_prev_safe
    // list_for_each_entry
    // list_for_each_entry_safe

    INIT_LIST_HEAD(&context->list);

    int32_t i;
    #define STUDENT_CNT (5)
    _student_t student[STUDENT_CNT];
    hy_s32_t id[STUDENT_CNT] = {5,4,3,2,1};
    for (i = 0; i < STUDENT_CNT; ++i) {
        _student_t *st = &student[i];

        HY_MEMSET(st->name, HY_STRING_BUF_MAX_LEN_32);
        snprintf(st->name, HY_STRING_BUF_MAX_LEN_32, "student%d", i);
        student[i].id = id[i];

        context->list_cnt++;
        list_add_tail(&st->list, &context->list);
    }

    _quick_sort(context);

    while (!context->exit_flag) {
        sleep(1);
    }

    _student_t *pos, *n;
    list_for_each_entry_safe(pos, n, &context->list, list) {
        LOGI("name: %s, id: %d \n", pos->name, pos->id);

        list_del(&pos->list);
    }

    _module_destroy(&context);

    return 0;
}

