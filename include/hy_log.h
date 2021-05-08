/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_log.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/08 2020 08:39
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/08 2020      create the file
 * 
 *     last modified: 06/08 2020 08:39
 */
#ifndef __LIBHY_UTILS_INCLUDE_HY_LOG_H_
#define __LIBHY_UTILS_INCLUDE_HY_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_type.h"

/**
 * @brief 打印等级定义
 */
typedef enum {
    HY_LOG_LEVEL_ERROR,
    HY_LOG_LEVEL_WARNING,
    HY_LOG_LEVEL_DEBUG,
    HY_LOG_LEVEL_INFO,

    HY_LOG_LEVEL_MAX
} HyLogLevel_t;

/**
 * @brief 真实输出log函数
 *
 * @param level: 该条语句的打印等级
 * @param tags: 打印类别
 * @param func: 对应的函数
 * @param line: 对应的行数
 * @param fmt: 具体打印的内容
 * @param ...
 *
 * @return 成功返回0
 *
 * note: 函数为内部用函数，最好不要在外面使用
 */
hy_int32_t HyLogWrite(HyLogLevel_t level, const char *tags, const char *func,
        hy_uint32_t line, char *fmt, ...);

void PrintHex(const char *tag, const char *name, hy_uint16_t line,
        const char *buf, hy_int32_t len, hy_int8_t flag);

/**
 * @brief 输出log宏转义
 *
 * note: 宏为内部用，最好不要在外面使用
 */
#define LOG(no_debug, level, fmt, ...)                                  \
    do {                                                                \
        if (no_debug) {                                                 \
            HyLogWrite(level, LOG_CATEGORY_TAG, __func__, __LINE__,     \
                    (char *)fmt, ##__VA_ARGS__);                        \
        }                                                               \
    } while (0)

/**
 * @brief 初始化log打印系统
 *
 * @param level: 打印等级，详见HyLogLevel_t
 *
 * @return 成功返回句柄
 */
void HyLogCreate(hy_uint8_t level, hy_uint32_t buf_len);

/**
 * @brief 销毁log系统
 */
void HyLogDestroy(void);

/**
 * @brief 输出对应的log等级函数
 */
#define	LOGE(fmt, ...)  LOG(ALONE_DEBUG, HY_LOG_LEVEL_ERROR,    fmt, ##__VA_ARGS__)
#define	LOGW(fmt, ...)  LOG(ALONE_DEBUG, HY_LOG_LEVEL_WARNING,  fmt, ##__VA_ARGS__)
#define	LOGD(fmt, ...)  LOG(ALONE_DEBUG, HY_LOG_LEVEL_DEBUG,    fmt, ##__VA_ARGS__)
#define	LOGI(fmt, ...)  LOG(ALONE_DEBUG, HY_LOG_LEVEL_INFO,     fmt, ##__VA_ARGS__)
#define PRINT_HEX_ASCII(buf, len) PrintHex(LOG_CATEGORY_TAG, __func__, __LINE__, buf, len, 1)
#define PRINT_HEX(buf, len) PrintHex(LOG_CATEGORY_TAG, __func__, __LINE__, buf, len, 0)

#ifdef __cplusplus
}
#endif

#endif

