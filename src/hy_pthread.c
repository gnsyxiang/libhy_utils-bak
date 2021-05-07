/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_pthread.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/12 2020 08:06
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/12 2020      create the file
 * 
 *     last modified: 05/12 2020 08:06
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hy_pthread.h"

#include "hy_log.h"

#ifdef USE_DEBUG
    #define ALONE_DEBUG 1
    #define LOG_CATEGORY_TAG "hy_pthread"
#endif

typedef struct {
    HyPthreadHandleCb_t handle_cb;
    pthread_t           id;
    int32_t             loop_flag;
} pthread_context_t;

static void *_pthread_cb_loop(void *args)
{
    if (!args) {
        LOGE("the param is NULL \n");
        return NULL;
    }
    pthread_context_t *context = args;
    HyPthreadHandleCb_t *handle_cb = &context->handle_cb;

    if (!handle_cb || !handle_cb->loop_cb) {
        LOGE("the thread loop func is NULL \n");
        return NULL;
    }

    while (context->loop_flag) {
        handle_cb->loop_cb(handle_cb->args);
    }

    return NULL;
}

void *HyPthreadCreate(HyPthreadHandleCb_t *handle_cb)
{
    if (!handle_cb) {
        LOGE("the param is NULL \n");
        return NULL;
    }

    pthread_context_t *context = calloc(1, sizeof(*context));
    if (!context) {
        LOGE("calloc faild \n");
        return NULL;
    }

    context->loop_flag = 1;
    memcpy(&context->handle_cb, handle_cb, sizeof(*handle_cb));

    pthread_create(&context->id, NULL, _pthread_cb_loop, context);

    return context;
}

void HyPthreadDestroy(void *handle)
{
    if (!handle) {
        LOGE("the param is NULL \n");
        return ;
    }
    pthread_context_t *context = handle;

    context->loop_flag = 0;
    if (0 != pthread_join(context->id, NULL)) {
        LOGE("pthread_join error \n");
    }

    free(context);
}

