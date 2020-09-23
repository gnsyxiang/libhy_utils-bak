/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_log.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/08 2020 08:42
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/08 2020      create the file
 * 
 *     last modified: 06/08 2020 08:42
 */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "hy_log.h"

#ifdef USE_DEBUG

typedef struct {
    char        *buf;
    uint32_t    len;

    uint8_t     level;
} log_context_t;
#define LOG_CONTEXT_T_LEN (sizeof(log_context_t))

static log_context_t log_context;

int32_t HyLogInit(char *buf, uint32_t len, uint8_t level)
{
    memset(&log_context, '\0', LOG_CONTEXT_T_LEN);

    log_context.buf = buf;
    log_context.len = len;

    log_context.level = level;

    return 0;
}

void HyLogDestory(void)
{
    memset(&log_context, '\0', LOG_CONTEXT_T_LEN);
}

int32_t _log_write(uint8_t level, const char *tags, const char *func, uint32_t line, char *fmt, ...)
{
    if (log_context.level < level) {
        return -1;
    }

    int32_t ret = 0;

    memset(log_context.buf, '\0', log_context.len);

    ret += snprintf(log_context.buf + ret, log_context.len - ret, "[%s][%s %d] ", tags, func, line); 

    va_list args;
    va_start(args, fmt);
    vsnprintf(log_context.buf + ret, log_context.len - ret, fmt, args);
    va_end(args);

    printf("%s", log_context.buf);

    return 0;
}
#else
int32_t HyLogInit(char *buf, uint32_t len, uint8_t level) {return 0;}
void HyLogDestory(void) {}
#endif

