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
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */

#define LIBUTILS_INC_THREAD_WRAPPER_GB
#include "thread_wrapper.h"
#undef LIBUTILS_INC_THREAD_WRAPPER_GB

#define thread_log(fmt, ...) \
    printf("<%s:%d, result: %s> " fmt, \
           __func__, __LINE__, strerror(errno), ##__VA_ARGS__);

/*
 * pthread_t pthread_self(void)     <进程级别>是pthread 库给每个线程定义的进程内唯一标识，是 pthread 库维护的，是进程级而非系统级
 * syscall(SYS_gettid)              <系统级别>这个系统全局唯一的“ID”叫做线程PID（进程ID），或叫做TID（线程ID），也有叫做LWP（轻量级进程=线程）的。
 */
static inline pid_t _getpid(void)
{
	return syscall(SYS_gettid);
}

// 设置的名字可以在proc文件系统中查看: cat /proc/PID/task/tid/comm
void Thread_SetName(pthread_t *thread, const char *name)
{
    int ret = -1;
    if (thread) {
        ret = prctl(PR_SET_NAME, name);
        if (ret != 0) {
            thread_log("prctl set name faild \n");
        }
    } else {
        // pthread_setname_np(*thread, name);
    }
}

void Thread_GetName(pthread_t *thread, char *name)
{
    int ret = -1;
    if (thread) {
        ret = prctl(PR_GET_NAME, name);
        if (ret != 0) {
            thread_log("prctl get name faild \n");
        }
    } else {
        // pthread_setname_np(*thread, name);
    }
}

static void *_thread_loop_wrapper(void *args)
{
    ThreadParam_t *thread_param = args;

    thread_param->pid = _getpid();
    Thread_SetName(&thread_param->id, thread_param->name);

    thread_param->thread_loop(thread_param->args);
    return NULL;
}

static inline int _check_thread_param(ThreadParam_t *thread_param)
{
    if (!thread_param) {
        thread_log("the thread_param is NULL");
        return -1;
    }
    if (!thread_param->thread_loop) {
        thread_log("the thread_loop is NULL");
        return -1;
    }
    return 0;
}

void Thread_CreateDetachedThread(ThreadParam_t *thread_param)
{
    if (0 != _check_thread_param(thread_param)) {
        return ;
    }

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (0 != pthread_create(&thread_param->id, &attr, \
                            _thread_loop_wrapper, thread_param)) {
        thread_log("pthread_create faild \n");
    }

    pthread_attr_destroy(&attr);
}

void Thread_CreateLowPriorityDetachedThread(ThreadParam_t *thread_param)
{
    if (0 != _check_thread_param(thread_param)) {
        return ;
    }

	int policy;
	pthread_attr_t attr;
	struct sched_param param;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	//设置继承的调度策略
	//必需设置inher的属性为PTHREAD_EXPLICIT_SCHED，否则设置线程的优先级会被忽略
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_getschedpolicy(&attr, &policy);
	param.sched_priority = sched_get_priority_min(policy);
	pthread_attr_setschedparam(&attr, &param);

    if (0 != pthread_create(&thread_param->id, &attr, \
                            _thread_loop_wrapper, thread_param)) {
        thread_log("pthread_create faild \n");
    }

    pthread_attr_destroy(&attr);
}

