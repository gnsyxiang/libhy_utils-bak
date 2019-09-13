/**
 *
 * Release under GPLv2.
 * 
 * @file    thread_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/08 2019 20:10
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/08 2019      create the file
 * 
 *     last modified: 08/08 2019 20:10
 */
#ifndef __LIBUTILS_INC_THREAD_WRAPPER_H_
#define __LIBUTILS_INC_THREAD_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

typedef void *(*thread_loop_t)(void *args);

typedef struct {
    pthread_t *id;
    thread_loop_t thread_loop;
    void *args;
} ThreadParam_t;
#define THREAD_PARAM_LEN (sizeof(ThreadParam_t))

void CreateAttachedThread(ThreadParam_t *thread_param);
void CreateLowPriorityAttachedThread(ThreadParam_t *thread_param);

#ifdef __cplusplus
}
#endif

#endif /* __LIBUTILS_INC_THREAD_WRAPPER_H_ */

