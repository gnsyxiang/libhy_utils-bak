/**
 * 
 * Release under GPLv2.
 * 
 * @file    signal_wrapper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    24/07 2019 13:52
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        24/07 2019      create the file
 * 
 *     last modified: 24/07 2019 13:52
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <execinfo.h>

#include "common.h"

#define LIBUTILS_SRC_SIGNAL_WRAPPER_GB
#include "signal_wrapper.h"
#undef LIBUTILS_SRC_SIGNAL_WRAPPER_GB

static char *g_app_name;

static void _dump_backtrace(void)
{
#define BACKTRACE_SIZE  (16)
    int size = 0;
    char **strings = NULL;
    void *array[BACKTRACE_SIZE];

    printf("Call Trace:\n");
    size = backtrace(array, BACKTRACE_SIZE);
    strings = backtrace_symbols(array, size);
    if (!strings) {
        printf("not found\n\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    for (i = 0; i < size; i++) {
      printf("  [%02d] %s\n", i, strings[i]);
    }
    free(strings);
}

static void _dump_maps(int signo)
{
#define CMD_LEN         (256)
    char cmd[CMD_LEN] = {0};

    if (signo == SIGINT || signo == SIGUSR1 || signo == SIGUSR2) {
        exit(-1);
    } else {
        system("mkdir -p ./coredump");
        sprintf(cmd, "cat /proc/%d/maps", getpid());
        printf("Process maps:\n");
        system(cmd);

        snprintf(cmd, CMD_LEN, "cat /proc/%d/maps > ./coredump/%s.%d.maps",
                getpid(), g_app_name, getpid());
        system(cmd);
    }
}

static void _sig_handler(int signo)
{
    char *signal_str[] = {
        [ 1] = "SIGHUP",      [ 2] = "SIGINT",      [ 3] = "SIGQUIT",     [ 4] = "SIGILL",
        [ 5] = "SIGTRAP",     [ 6] = "SIGABRT",     [ 7] = "",            [ 8] = "SIGFPE",
        [ 9] = "SIGKILL",     [10] = "SIGUBS",      [11] = "SIGSEGV",     [12] = "SIGSYS",
        [13] = "SIGPIPE",     [14] = "SIGALRM",     [15] = "SIGTERM",     [16] = "SIGUSR1",
        [17] = "SIGUSR2",     [18] = "SIGCHLD",     [19] = "SIGPWR",      [20] = "SIGWINCH",
        [21] = "SIGURG",      [22] = "SIGPOLL",     [23] = "SIGSTOP",     [24] = "SIGTSTP",
        [25] = "SIGCONT",     [26] = "SIGTTIN",     [27] = "SIGTTOU",     [28] = "SIGVTALRM",
        [29] = "SIGPROF",     [30] = "SIGXCPU",     [31] = "SIGXFSZ",
    };

    printf("\n\n[%s] %s(%d) crashed by signal %s.\n",
           __func__, g_app_name, getpid(), signal_str[signo]);

    _dump_backtrace();
    _dump_maps(signo);

    SignalHandleFinal();
}

void SignalHandleInit(const char * const app_name)
{
    int sig_num[] = {
        SIGQUIT, SIGILL,  SIGTRAP, SIGABRT, SIGFPE, SIGSEGV, SIGBUS,
        SIGSYS,  SIGXCPU, SIGXFSZ, SIGINT, 

        SIGUSR1, SIGUSR2, 
        SIGPIPE,
    };

    g_app_name = strdup(app_name);

    struct sigaction act;
    act.sa_flags = SA_RESETHAND;
    sigemptyset(&act.sa_mask);
    act.sa_handler = _sig_handler;

    int i;
    for (i = 0; i < ARRAY_CNT(sig_num); i++) {
        sigaction(sig_num[i], &act, NULL);
    }
}

void SignalHandleFinal(void)
{
    free(g_app_name);
}
