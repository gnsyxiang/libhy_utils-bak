/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_timerwheel_rotation.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    16/07 2021 14:24
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        16/07 2021      create the file
 * 
 *     last modified: 16/07 2021 14:24
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#include "hy_timerwheel_rotation.h"

#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_list.h"
#include "hy_log.h"

#include "hy_hal/hy_time.h"

#define ALONE_DEBUG 1

typedef struct {
    HyTimerHandleConfig_t timer_config;

    size_t rotation;

    struct list_head list;
} _timer_t;

typedef struct {
    uint32_t cur_slot;
    uint32_t slot_num;
    uint32_t slot_interval;

    pthread_t thread_id;
    int exit_flag;

    struct list_head *list_head;
} _timerwheel_context_t;

void *HyTimerWheelRotationAdd(void *handle, HyTimerHandleConfig_t *timer_config)
{
    ASSERT_NULL_RET_VAL(!handle || !timer_config, NULL);
    _timerwheel_context_t *context = handle;

    _timer_t *timer = HY_MALLOC_RET_VAL(sizeof(*timer), NULL);

    HY_MEMCPY(&timer->timer_config, timer_config);

    timer->rotation = timer_config->expires / context->slot_num;
    size_t slot     = timer_config->expires % context->slot_num;

    list_add_tail(&timer->list, &context->list_head[slot]);

    return timer;
}

void HyTimerWheelRotationDel(void *handle, void *timer_handle)
{
    ASSERT_NULL_RET(!handle || !timer_handle);

    _timerwheel_context_t *context = handle;
    _timer_t *timer = timer_handle;

    size_t slot = timer->timer_config.expires % context->slot_num;

    _timer_t *pos, *n;
    list_for_each_entry_safe(pos, n, &context->list_head[slot], list) {
        if (timer_handle == pos) {
            list_del(&pos->list);

            HY_FREE(&pos);
            break;
        }
    }
}

static void *_timer_wheel_loop(void *args)
{
    _timerwheel_context_t *context = args;

    while (!context->exit_flag) {
        struct timeval tv;
        tv.tv_sec   = 1;
        tv.tv_usec  = 0;

        int err;
        do {
            err = select(0, NULL, NULL, NULL, &tv);
        } while(err < 0 && errno == EINTR);

        _timer_t *pos, *n;
        list_for_each_entry_safe(pos, n, &context->list_head[context->cur_slot], list) {
            if (pos->rotation > 0) {
                pos->rotation--;
            } else {
                HyTimerHandleConfig_t *timer_config = &pos->timer_config;
                if (timer_config->timer_cb) {
                    timer_config->timer_cb(timer_config->args);
                }
                list_del(&pos->list);

                HY_FREE(&pos);
            }
        }

        context->cur_slot++;
    }

    return NULL;
}

void HyTimerWheelRotationDestroy(void **handle)
{
    LOGT("%s:%d \n", __func__, __LINE__);
    ASSERT_NULL_RET(!handle || !*handle);

    _timerwheel_context_t *context = *handle;

    context->exit_flag = 1;
    pthread_join(context->thread_id, NULL);

    _timer_t *pos, *n;
    for (uint32_t i = 0; i < context->slot_num; ++i) {
        list_for_each_entry_safe(pos, n, &context->list_head[i], list) {
            list_del(&pos->list);

            HY_FREE(&pos);
        }
    }

    HY_FREE(&context->list_head);

    HY_FREE(handle);
}

void *HyTimerWheelRotationCreate(HyTimerWheelRotationConfig_t *timerwheel_rotation_config)
{
    LOGT("%s:%d \n", __func__, __LINE__);
    _timerwheel_context_t *context = NULL;

    do {
        context = HY_MALLOC_BREAK(sizeof(*context));
        context->list_head = HY_MALLOC_BREAK(sizeof(struct list_head) * timerwheel_rotation_config->slot_num);

        context->slot_interval = timerwheel_rotation_config->slot_interval;
        context->slot_num = timerwheel_rotation_config->slot_num;

        for (uint32_t i = 0; i < timerwheel_rotation_config->slot_num; ++i) {
            INIT_LIST_HEAD(&context->list_head[i]);
        }

        if (0 != pthread_create(&context->thread_id, NULL, _timer_wheel_loop, context)) {
            LOGE("pthread_create faild \n");
            break;
        }

        return context;
    } while (0);

    HyTimerWheelRotationDestroy((void **)&context);

    return NULL;
}

