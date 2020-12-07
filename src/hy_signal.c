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

#include <sys/types.h>
#include <unistd.h>
#include <execinfo.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "hy_log.h"

#ifdef USE_DEBUG
    #define ALONE_DEBUG 1
    #define LOG_CATEGORY_TAG "hy_signal"
#endif

static char *global_app_name;

static char *signal_str[] = {
    [1] = "SIGHUP",       [2] = "SIGINT",       [3] = "SIGQUIT",      [4] = "SIGILL",
    [5] = "SIGTRAP",      [6] = "SIGABRT",      [7] = "",             [8] = "SIGFPE",
    [9] = "SIGKILL",      [10] = "SIGUBS",      [11] = "SIGSEGV",     [12] = "SIGSYS",
    [13] = "SIGPIPE",     [14] = "SIGALRM",     [15] = "SIGTERM",     [16] = "SIGUSR1",
    [17] = "SIGUSR2",     [18] = "SIGCHLD",     [19] = "SIGPWR",      [20] = "SIGWINCH",
    [21] = "SIGURG",      [22] = "SIGPOLL",     [23] = "SIGSTOP",     [24] = "SIGTSTP",
    [25] = "SIGCONT",     [26] = "SIGTTIN",     [27] = "SIGTTOU",     [28] = "SIGVTALRM",
    [29] = "SIGPROF",     [30] = "SIGXCPU",     [31] = "SIGXFSZ",
};

       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
extern char *in_char;
extern char *out_char;
extern int in_len;
extern int in_after_len;
extern int out_len;
int in_fd = -1;
int out_fd = -1;

#include "hy_fifo.h"
extern fifo_context_t *context_tmp;

static void sig_handler(int signo)
{
    char cmd[256] = {0};
    void *array[10];
    int size = 0;
    char **strings = NULL;
    int i = 0;

    printf("\n\n[%s] %s(%d) crashed by signal %s.\n",
           __func__, global_app_name, getpid(), signal_str[signo]);

    in_fd = open("./in.hex", O_RDWR|O_CREAT|O_TRUNC ,0777);
    out_fd = open("./out.hex", O_RDWR|O_CREAT|O_TRUNC ,0777);

    write(in_fd, in_char, in_len);
    write(out_fd, out_char, out_len);

    HyFifoDump(context_tmp);

    printf("Call Trace:\n");
#ifndef NO_backtrace
    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);
#endif
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
        system("mkdir -p /mnt/sdcard/coredump");
        sprintf(cmd, "cat /proc/%d/maps", getpid());
        printf("Process maps:\n");
        system(cmd);

        snprintf(cmd, 256, "cat /proc/%d/maps > /mnt/sdcard/coredump/%s.%d.maps",
             getpid(), global_app_name, getpid());
        system(cmd);
    }
}

void HySignalInit(const char *appname)
{
    static struct sigaction act;

    global_app_name = strdup(appname);

    act.sa_flags = SA_RESETHAND;
    sigemptyset(&act.sa_mask);
    act.sa_handler = sig_handler;

    sigaction(SIGQUIT, &act, NULL);
    sigaction(SIGILL, &act, NULL);
    sigaction(SIGTRAP, &act, NULL);
    sigaction(SIGABRT, &act, NULL);
    sigaction(SIGFPE, &act, NULL);
    sigaction(SIGSEGV, &act, NULL);
    sigaction(SIGBUS, &act, NULL);
    sigaction(SIGSYS, &act, NULL);
    sigaction(SIGXCPU, &act, NULL);
    sigaction(SIGXFSZ, &act, NULL);

    sigaction(SIGINT, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);

    signal(SIGPIPE, SIG_IGN);
}

