/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_timer.c
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

#include "hy_timer.h"

#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_list.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

typedef struct {
    HyTimerConfig_t timer_config;

    size_t rotation;

    struct list_head list;
} _timer_t;

typedef struct {
    HyTimerServiceSaveConfig_t save_config;

    uint32_t cur_slot;

    pthread_t id;
    int exit_flag;

    struct list_head *list_head;
} _timer_context_t;

static _timer_context_t *context = NULL;

void *HyTimerAdd(HyTimerConfig_t *timer_config)
{
    HY_ASSERT_VAL_RET_VAL(!timer_config, NULL);

    _timer_t *timer = HY_MALLOC_RET_VAL(_timer_t *, sizeof(*timer), NULL);

    HY_MEMCPY(&timer->timer_config, timer_config);

    timer->rotation = timer_config->expires / context->save_config.slot_num;
    size_t slot     = timer_config->expires % context->save_config.slot_num;

    list_add_tail(&timer->list, &context->list_head[slot]);

    return timer;
}

void HyTimerDel(void *timer_handle)
{
    HY_ASSERT_VAL_RET(!timer_handle);

    _timer_t *timer = timer_handle;

    size_t slot = timer->timer_config.expires % context->save_config.slot_num;

    _timer_t *pos, *n;
    list_for_each_entry_safe(pos, n, &context->list_head[slot], list) {
        if (timer_handle == pos) {
            list_del(&pos->list);

            HY_FREE_PP(&pos);
            break;
        }
    }
}

static void *_timer_loop_cb(void *args)
{
    struct timeval tv;
    int err;
    _timer_t *pos, *n;
    time_t sec, usec;
    HyTimerConfig_t *timer_config = NULL;

    sec = context->save_config.slot_interval_ms / 1000;
    usec = (context->save_config.slot_interval_ms % 1000) * 1000;

    while (!context->exit_flag) {
        tv.tv_sec   = sec;
        tv.tv_usec  = usec;

        do {
            err = select(0, NULL, NULL, NULL, &tv);
        } while(err < 0 && errno == EINTR);

        list_for_each_entry_safe(pos, n, &context->list_head[context->cur_slot], list) {
            if (pos->rotation > 0) {
                pos->rotation--;
            } else {
                timer_config = &pos->timer_config;
                if (timer_config->timer_cb) {
                    timer_config->timer_cb(timer_config->args);
                }
                list_del(&pos->list);

                if (pos->timer_config.repeat_flag) {
                    pos->rotation = timer_config->expires / context->save_config.slot_num;
                    size_t slot     = timer_config->expires % context->save_config.slot_num;
                    slot += context->cur_slot;
                    slot %= context->save_config.slot_num;

                    list_add_tail(&pos->list, &context->list_head[slot]);
                } else {
                    HY_FREE_PP(&pos);
                }
            }
        }

        context->cur_slot++;
        context->cur_slot %= context->save_config.slot_num;
    }

    return NULL;
}

void HyTimerDestroy(void)
{
    context->exit_flag = 1;
    pthread_join(context->id, NULL);

    _timer_t *pos, *n;
    for (uint32_t i = 0; i < context->save_config.slot_num; ++i) {
        list_for_each_entry_safe(pos, n, &context->list_head[i], list) {
            list_del(&pos->list);

            HY_FREE_PP(&pos);
        }
    }

    HY_FREE_PP(&context->list_head);

    HY_FREE_PP(&context);

    LOGI("timer destroy successful \n");
}

void HyTimerCreate(HyTimerServiceConfig_t *config)
{
    HY_ASSERT_VAL_RET(!config);

    do {
        context = HY_MALLOC_BREAK(_timer_context_t *, sizeof(*context));
        context->list_head = HY_MALLOC_BREAK(struct list_head *,
                sizeof(struct list_head) * config->save_config.slot_num);

        HY_MEMCPY(&context->save_config, &config->save_config);

        for (uint32_t i = 0; i < config->save_config.slot_num; ++i) {
            INIT_LIST_HEAD(&context->list_head[i]);
        }

        pthread_create(&context->id, NULL, _timer_loop_cb, context);

        LOGI("timer create successful \n");
        return;
    } while (0);

    HyTimerDestroy();
}
