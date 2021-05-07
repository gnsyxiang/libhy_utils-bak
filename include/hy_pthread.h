/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_pthread.h
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
#ifndef __LIBUTILS_INCLUDE_HY_PTHREAD_H_
#define __LIBUTILS_INCLUDE_HY_PTHREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <pthread.h>

typedef void (*HyPthreadLoopCb_t)(void *args);
typedef struct {
    HyPthreadLoopCb_t   loop_cb;
    void                *args;
} HyPthreadHandleCb_t;

void *HyPthreadCreate(HyPthreadHandleCb_t *handle_cb);
void HyPthreadDestroy(void *handle);

#ifdef __cplusplus
}
#endif

#endif

