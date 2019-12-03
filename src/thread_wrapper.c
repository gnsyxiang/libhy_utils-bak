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
#include <stdlib.h>

#define LIBUTILS_INC_THREAD_WRAPPER_GB
#include "thread_wrapper.h"
#undef LIBUTILS_INC_THREAD_WRAPPER_GB
#include "utils.h"

#define thread_log(fmt, ...) \
    printf("<%s:%d, result: %s> " fmt, \
           __func__, __LINE__, strerror(errno), ##__VA_ARGS__);

#define THREAD_WRAPPER_NAME_LEN (16)
typedef struct {
    ThreadParam_t   *param;

    pthread_t       id;
    pid_t           pid;
    char            name[THREAD_WRAPPER_NAME_LEN];
} thread_wrapper_state_t;

/*
 * pthread_t pthread_self(void)     <进程级别>是pthread 库给每个线程定义的进程内唯一标识，是 pthread 库维护的，是进程级而非系统级
 * syscall(SYS_gettid)              <系统级别>这个系统全局唯一的“ID”叫做线程PID（进程ID），或叫做TID（线程ID），也有叫做LWP（轻量级进程=线程）的。
 */
static inline pid_t _getpid(void)
{
	return syscall(SYS_gettid);
}

// 设置的名字可以在proc文件系统中查看: cat /proc/PID/task/tid/comm
void Thread_SetName(const char *name)
{
    pthread_t *thread = NULL;
    if (!name) {
        thread_log("the name is NULL \n");
        return;
    }

    int ret = -1;
    if (!thread) {
        ret = prctl(PR_SET_NAME, name);
        if (ret != 0) {
            thread_log("prctl set name faild \n");
        }
    } else {
        // pthread_setname_np(*thread, name);
    }
}

void Thread_GetName(char *name)
{
    pthread_t *thread = NULL;
    int ret = -1;
    if (!thread) {
        ret = prctl(PR_GET_NAME, name);
        if (ret != 0) {
            thread_log("prctl get name faild \n");
        }
    } else {
        // pthread_setname_np(*thread, name);
    }
}

static thread_wrapper_state_t *_copy_param_init(ThreadParam_t *thread_param)
{
    thread_wrapper_state_t *state = calloc(1, DATA_TYPE_LEN(*state));
    if (!state) {
        thread_log("calloc faild \n");
        return NULL;
    }

    state->param = calloc(1, DATA_TYPE_LEN(ThreadParam_t));
    if (!state->param) {
        thread_log("calloc faild \n");
        return NULL;
    }

    memcpy(state->param, thread_param, DATA_TYPE_LEN(ThreadParam_t));

    int len = strlen(thread_param->name);
    len = (len > THREAD_WRAPPER_NAME_LEN) ? THREAD_WRAPPER_NAME_LEN - 1 : len;
    memcpy(state->name, thread_param->name, len);

    return state;
}

static inline void _copy_param_final(thread_wrapper_state_t *state)
{
    if (state) {
        if (state->param) {
            free(state->param);
        }
        free(state);
    }
}

static void *_thread_loop_wrapper(void *args)
{
    thread_wrapper_state_t *state = args;

    state->pid = _getpid();
    Thread_SetName(state->name);

    state->param->thread_loop(state->param->args);

    _copy_param_final(state);
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

    thread_wrapper_state_t *state = _copy_param_init(thread_param);
    if (!state) {
        thread_log("_copy_param_init faild \n");
        return ;
    }

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (0 != pthread_create(&state->id, &attr, _thread_loop_wrapper, state)) {
        thread_log("pthread_create faild \n");
    }

    pthread_attr_destroy(&attr);
}

void Thread_CreateLowPriorityDetachedThread(ThreadParam_t *thread_param)
{
    if (0 != _check_thread_param(thread_param)) {
        return ;
    }

    thread_wrapper_state_t *state = _copy_param_init(thread_param);
    if (!state) {
        thread_log("_copy_param_init faild \n");
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

    if (0 != pthread_create(&state->id, &attr, _thread_loop_wrapper, state)) {
        thread_log("pthread_create faild \n");
    }

    pthread_attr_destroy(&attr);
}

