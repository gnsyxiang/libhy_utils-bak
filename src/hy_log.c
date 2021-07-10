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
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <inttypes.h>

#include "hy_log.h"
#include "hy_utils.h"

#include "hy_hal/hy_type.h"

typedef struct {
    hy_uint32_t buf_len;
    char        *buf;

    hy_uint32_t cur_len;

    hy_uint8_t  level;
} log_context_t;

static log_context_t *context = NULL;

void *HyLogCreate(HyLogConfig_t *log_config)
{
    if (!log_config) {
        return NULL;
    }

    context = (log_context_t *)calloc(1, sizeof(*context));
    if (!context) {
        printf("calloc faild \n");
        return NULL;
    }

    context->buf = (char *)calloc(1, log_config->buf_len);
    if (!context->buf) {
        printf("buf_union create faild \n");
        free(context);
        return NULL;
    }

    context->buf_len    = log_config->buf_len;
    context->level      = log_config->level;

    return context;
}

void HyLogDestroy(void *handle)
{
    if (context) {
        if (context->buf) {
            free(context->buf);
        }
        free(context);
    }
}

/*设置输出前景色*/
#define PRINT_FONT_BLA      "\033[30m"      //黑色
#define PRINT_FONT_RED      "\033[31m"      //红色
#define PRINT_FONT_GRE      "\033[32m"      //绿色
#define PRINT_FONT_YEL      "\033[33m"      //黄色
#define PRINT_FONT_BLU      "\033[34m"      //蓝色
#define PRINT_FONT_PUR      "\033[35m"      //紫色
#define PRINT_FONT_CYA      "\033[36m"      //青色
#define PRINT_FONT_WHI      "\033[37m"      //白色

/*设置输出背景色*/ 
#define PRINT_BACK_BLA      "\033[40m"      //黑色
#define PRINT_BACK_RED      "\033[41m"      //红色
#define PRINT_BACK_GRE      "\033[42m"      //绿色
#define PRINT_BACK_YEL      "\033[43m"      //黄色
#define PRINT_BACK_BLU      "\033[44m"      //蓝色
#define PRINT_BACK_PUR      "\033[45m"      //紫色
#define PRINT_BACK_CYA      "\033[46m"      //青色
#define PRINT_BACK_WHI      "\033[47m"      //白色

/*输出属性设置*/
#define PRINT_ATTR_RESET    "\033[0m"       //重新设置属性到缺省设置 
#define PRINT_ATTR_BOL      "\033[1m"       //设置粗体 
#define PRINT_ATTR_LIG      "\033[2m"       //设置一半亮度(模拟彩色显示器的颜色) 
#define PRINT_ATTR_LIN      "\033[4m"       //设置下划线(模拟彩色显示器的颜色) 
#define PRINT_ATTR_GLI      "\033[5m"       //设置闪烁 
#define PRINT_ATTR_REV      "\033[7m"       //设置反向图象 
#define PRINT_ATTR_THI      "\033[22m"      //设置一般密度 
#define PRINT_ATTR_ULIN     "\033[24m"      //关闭下划线 
#define PRINT_ATTR_UGLI     "\033[25m"      //关闭闪烁 
#define PRINT_ATTR_UREV     "\033[27m"      //关闭反向图象

// printf("\033[字背景颜色;字体颜色m字符串\033[0m" );

static inline void _output_set_color(HyLogLevel_t level, hy_uint32_t *ret)
{
#if 0
    hy_char_t *color[HY_LOG_LEVEL_MAX][2] = {
        {"E", PRINT_FONT_RED},
        {"W", PRINT_FONT_GRE},
        {"D", ""},
        {"I", ""},
    };

    *ret += snprintf(context->buf + *ret, context->buf_len - *ret, "%s[%s]", color[level][1], color[level][0]);
#endif
}

static inline void _output_reset_color(HyLogLevel_t level, hy_uint32_t *ret)
{
#if 0
    *ret += snprintf(context->buf + *ret, context->buf_len - *ret, "%s", PRINT_ATTR_RESET);
#endif
}

void HyLogWrite(int level, const char *file, const char *func,
        uint32_t line, char *fmt, ...)
{
    if (context && context->level <= level) {
        hy_uint32_t ret = 0;
        memset(context->buf, '\0', context->buf_len);

        _output_set_color(level, &ret);

        ret += snprintf(context->buf + ret, context->buf_len - ret, "[%s:%"PRId32"][%s] ", file, line, func); 

        va_list args;
        va_start(args, fmt);
        ret += vsnprintf(context->buf + ret, context->buf_len - ret, fmt, args);
        va_end(args);

        _output_reset_color(level, &ret);

        printf("%s", (char *)context->buf);
    }
}

void HyPrintHex(const char *name, uint16_t line,
        const char *buf, size_t len, int8_t flag)
{
    if (len <= 0) {
        return;
    }

    hy_uint8_t cnt = 0;
    printf("[%s %d]len: %zu \r\n", name, line, len);
    for (size_t i = 0; i < len; i++) {
        if (flag == 1) {
            if (buf[i] == 0x0d || buf[i] == 0x0a || buf[i] < 32 || buf[i] >= 127) {
                printf("%02x[ ]  ", (hy_uint8_t)buf[i]);
            } else {
                printf("%02x[%c]  ", (hy_uint8_t)buf[i], (hy_uint8_t)buf[i]);
            }
        } else {
            printf("%02x ", (hy_uint8_t)buf[i]);
        }
        cnt++;
        if (cnt == 16) {
            cnt = 0;
            printf("\r\n");
        }
    }
    printf("\r\n");
}

