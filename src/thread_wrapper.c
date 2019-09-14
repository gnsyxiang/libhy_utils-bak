/**
 * 
 * Release under GPLv2.
 * 
 * @file    thread_wrapper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/08 2019 20:16
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/08 2019      create the file
 * 
 *     last modified: 08/08 2019 20:16
 */
#include <stdio.h>
#include <sched.h>
#include <assert.h>

#include "thread_wrapper.h"

static void _create_thread_common(ThreadParam_t *thread_param, pthread_attr_t *attr)
{
    pthread_t t;
    if (!thread_param->id)
        thread_param->id = &t;

    int error = pthread_create(thread_param->id,
                               attr,
                               thread_param->thread_loop,
                               thread_param->args);

    pthread_attr_destroy(attr);

    assert(!error);
}

void CreateAttachedThread(ThreadParam_t *thread_param)
{
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    _create_thread_common(thread_param, &attr);
}

void CreateLowPriorityAttachedThread(ThreadParam_t *thread_param)
{
	int policy;
	struct sched_param param;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	//设置继承的调度策略
	//必需设置inher的属性为PTHREAD_EXPLICIT_SCHED，否则设置线程的优先级会被忽略
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

	pthread_attr_getschedpolicy(&attr, &policy);
	param.sched_priority = sched_get_priority_min(policy);
	pthread_attr_setschedparam(&attr, &param);

    _create_thread_common(thread_param, &attr);
}

