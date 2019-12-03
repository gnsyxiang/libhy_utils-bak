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

#ifndef LIBUTILS_INC_THREAD_WRAPPER_GB
#define THREAD_WRAPPER_EX extern
#else
#define THREAD_WRAPPER_EX
#endif

typedef void (*ThreadLoop_t)(void *args);

/**
 * @brief 
 * @param name: 设置的名字，最好不要超过16字节
 */
typedef struct {
    const char      *name;
    ThreadLoop_t    thread_loop;
    void            *args;
} ThreadParam_t;

/**
 * @brief 
 *
 * @param thread_param
 *
 * @return 
 */
THREAD_WRAPPER_EX void Thread_CreateDetachedThread(ThreadParam_t *thread_param);
THREAD_WRAPPER_EX void Thread_CreateLowPriorityDetachedThread(ThreadParam_t *thread_param);

THREAD_WRAPPER_EX void Thread_SetName(const char *name);
THREAD_WRAPPER_EX void Thread_GetName(char *name);

#ifdef __cplusplus
}
#endif

#endif /* __LIBUTILS_INC_THREAD_WRAPPER_H_ */

