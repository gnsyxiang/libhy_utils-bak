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
#include <stdlib.h>

#include "hy_log.h"
#include "hy_utils.h"

#ifdef USE_DEBUG

#define ANSI_COLOR_RED     "\x1b[1;31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct {
    uint32_t    buf_len;
    BufUnion_t  *buf;

    uint8_t     level;
} log_context_t;
#define LOG_CONTEXT_T_LEN (sizeof(log_context_t))

static log_context_t *context = NULL;

int32_t HyLogWrite(uint8_t level, const char *tags, const char *func,
        uint32_t line, char *fmt, ...)
{
#define SNPRINTF_FMT    context->buf->buf + ret, context->buf_len - ret
    if (context->level < level) {
        return -1;
    }

    uint32_t ret = 0;
    memset(context->buf->buf, '\0', context->buf_len);

    // if (level <= LOG_LEVEL_WARNING) {
        // ret += snprintf(SNPRINTF_FMT, ANSI_COLOR_RED);
    // }

    ret += snprintf(SNPRINTF_FMT, "[%s][%s %d] ", tags, func, line); 

    va_list args;
    va_start(args, fmt);
    ret += vsnprintf(SNPRINTF_FMT, fmt, args);
    va_end(args);

    // if (level <= LOG_LEVEL_WARNING) {
        // ret += snprintf(SNPRINTF_FMT, ANSI_COLOR_RESET);
    // }

    printf("%s", (char *)context->buf->buf);

    return 0;
}

void HyLogCreate(uint8_t level, uint32_t buf_len)
{
    context = calloc(1, sizeof(*context));
    if (!context) {
        printf("calloc faild \n");
        return ;
    }

    context->buf = HyBufUnionCreate(buf_len);
    if (!context->buf) {
        printf("buf_union create faild \n");
        free(context);
        return ;
    }

    context->buf_len    = buf_len;
    context->level      = level;
}

void HyLogDestory(void)
{
    if (context->buf) {
        HyBufUnionDestroy(context->buf);
    }

    free(context);
}

void PrintHex(const char *tag, const char *name, uint16_t line,
        char *buf, int32_t len, int8_t flag)
{
    if (len <= 0) {
        return;
    }
    uint8_t cnt = 0;
    printf("[%s][%s %d]len: %d \r\n", tag, name, line, len);
    for (int i = 0; i < len; i++) {
        if (flag == 1) {
            if (buf[i] == 0x0d || buf[i] == 0x0a || buf[i] < 32 || buf[i] >= 127) {
                printf("%02x[ ]  ", (uint8_t)buf[i]);
            } else {
                printf("%02x[%c]  ", (uint8_t)buf[i], (uint8_t)buf[i]);
            }
        } else {
            printf("%02x ", (uint8_t)buf[i]);
        }
        cnt++;
        if (cnt == 16) {
            cnt = 0;
            printf("\r\n");
        }
    }
    printf("\r\n");
}
#else
void HyLogCreate(uint8_t level, uint32_t buf_len) {return;}
void HyLogDestory(void) {}
int32_t HyLogWrite(uint8_t level, const char *tags, const char *func,
        uint32_t line, char *fmt, ...) {return 0;}
void PrintHex(const char *tag, const char *name, uint16_t line,
        char *buf, int32_t len, int8_t flag) {}
#endif

