/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_signal.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/12 2020 14:27
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/12 2020      create the file
 * 
 *     last modified: 05/12 2020 14:27
 */
#include <stdio.h>

#include "hy_signal.h"

#if (__linux__ && __GLIBC__ && !__UCLIBC__) || __APPLE__

#include <sys/types.h>
#include <unistd.h>
#include <execinfo.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "hy_log.h"
#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_string.h"

#define ALONE_DEBUG 1

typedef struct {
    HySignalConfig_t signal_config;
} _signal_context_t;

static _signal_context_t *context = NULL;

static char *signal_str[] = {
    [1] =  "SIGHUP",      [2] =  "SIGINT",      [3] =  "SIGQUIT",     [4] =  "SIGILL",
    [5] =  "SIGTRAP",     [6] =  "SIGABRT",     [7] =  "",            [8] =  "SIGFPE",
    [9] =  "SIGKILL",     [10] = "SIGUBS",      [11] = "SIGSEGV",     [12] = "SIGSYS",
    [13] = "SIGPIPE",     [14] = "SIGALRM",     [15] = "SIGTERM",     [16] = "SIGUSR1",
    [17] = "SIGUSR2",     [18] = "SIGCHLD",     [19] = "SIGPWR",      [20] = "SIGWINCH",
    [21] = "SIGURG",      [22] = "SIGPOLL",     [23] = "SIGSTOP",     [24] = "SIGTSTP",
    [25] = "SIGCONT",     [26] = "SIGTTIN",     [27] = "SIGTTOU",     [28] = "SIGVTALRM",
    [29] = "SIGPROF",     [30] = "SIGXCPU",     [31] = "SIGXFSZ",
};

static void _signal_handler(int signo)
{
    char cmd[256] = {0};
    void *array[10];
    int size = 0;
    char **strings = NULL;
    int i = 0;
    HySignalConfig_t *config = &context->signal_config;

    if (config->handle_cb) {
        config->handle_cb(config->args);
    }

    printf("\n\n[%s] %s(%d) crashed by signal %s.\n",
           __func__, config->appname, getpid(), signal_str[signo]);

    printf("Call Trace:\n");
    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);
    if (strings) {
        for (i = 0; i < size; i++)
            printf("  %s\n", strings[i]);
        free(strings);
    } else {
        printf("Not Found\n\n");
    }

    if (signo == SIGINT || signo == SIGUSR1 || signo == SIGUSR2) {
        exit(-1);
    } else {
        sprintf(cmd, "mkdir -p %s", config->coredump_path);
        system(cmd);
        sprintf(cmd, "cat /proc/%d/maps", getpid());
        printf("Process maps:\n");
        system(cmd);

        snprintf(cmd, 256, "cat /proc/%d/maps > %s/%s.%d.maps",
             getpid(), config->coredump_path, config->appname, getpid());
        system(cmd);
    }
}

void HySignalDestroy(void **handle)
{
    LOGT("%s:%d \n", __func__, __LINE__);
    HY_ASSERT_NULL_RET(!handle || !*handle);

    HY_FREE(handle);
}

void *HySignalCreate(HySignalConfig_t *signal_config)
{
    LOGT("%s:%d \n", __func__, __LINE__);
    HY_ASSERT_NULL_RET_VAL(!signal_config, NULL);

    static struct sigaction act;

    do {
        context = HY_MALLOC_BREAK(sizeof(*context));
        HY_MEMCPY(&context->signal_config, signal_config);

        act.sa_flags = SA_RESETHAND;
        sigemptyset(&act.sa_mask);
        act.sa_handler = _signal_handler;

        sigaction(SIGQUIT, &act, NULL);
        sigaction(SIGILL,  &act, NULL);
        sigaction(SIGTRAP, &act, NULL);
        sigaction(SIGABRT, &act, NULL);
        sigaction(SIGFPE,  &act, NULL);
        sigaction(SIGSEGV, &act, NULL);
        sigaction(SIGBUS,  &act, NULL);
        sigaction(SIGSYS,  &act, NULL);
        sigaction(SIGXCPU, &act, NULL);
        sigaction(SIGXFSZ, &act, NULL);

        sigaction(SIGINT,  &act, NULL);
        sigaction(SIGUSR1, &act, NULL);
        sigaction(SIGUSR2, &act, NULL);

        signal(SIGPIPE, SIG_IGN);

        return context;
    } while (0);

    HySignalDestroy((void **)&context);
    return NULL;
}
#else
void *HySignalCreate(HySignalConfig_t *signal_config) {return NULL;}
void HySignalDestroy(void **handle) {}
#endif

