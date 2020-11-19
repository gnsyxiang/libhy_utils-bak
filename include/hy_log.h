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
#ifndef __LIBUTILS_INCLUDE_HY_LOG_H_
#define __LIBUTILS_INCLUDE_HY_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// #define INTRANET_SERVER_TEST

#define USE_DEBUG

#ifdef USE_DEBUG
/**
 * @brief 打印等级定义
 */
#define LOG_LEVEL_ERROR         (1)
#define LOG_LEVEL_WARNING       (2)
#define LOG_LEVEL_DEBUG         (3)
#define LOG_LEVEL_INFO          (4)
#define LOG_LEVEL_MAX           (5)

// #define USE_LOG_LEVEL           (LOG_LEVEL_INFO)
#define USE_LOG_LEVEL           (LOG_LEVEL_DEBUG)
// #define USE_LOG_LEVEL           (LOG_LEVEL_WARNING)

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
int32_t HyLogWrite(uint8_t level, const char *tags, const char *func,
        uint32_t line, char *fmt, ...);

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

void PrintHex(const char *tag, const char *name, uint16_t line,
        char *buf, int32_t len, int8_t flag);
#endif

/**
 * @brief 初始化log打印系统
 *
 * @param level: 打印等级，详见LogLevel_t
 *
 * @return 成功返回句柄
 */
void HyLogCreate(uint8_t level, uint32_t buf_len);

/**
 * @brief 销毁log系统
 */
void HyLogDestory(void);

/**
 * @brief 输出对应的log等级函数
 */
#ifdef USE_DEBUG
    #define	LOGE(fmt, ...)  LOG(ALONE_DEBUG, LOG_LEVEL_ERROR,    fmt, ##__VA_ARGS__)
    #define	LOGW(fmt, ...)  LOG(ALONE_DEBUG, LOG_LEVEL_WARNING,  fmt, ##__VA_ARGS__)
    #define	LOGD(fmt, ...)  LOG(ALONE_DEBUG, LOG_LEVEL_DEBUG,    fmt, ##__VA_ARGS__)
    #define	LOGI(fmt, ...)  LOG(ALONE_DEBUG, LOG_LEVEL_INFO,     fmt, ##__VA_ARGS__)
    #define PRINT_HEX_ASCII(buf, len) PrintHex(LOG_CATEGORY_TAG, __func__, __LINE__, buf, len, 1)
    #define PRINT_HEX(buf, len) PrintHex(LOG_CATEGORY_TAG, __func__, __LINE__, buf, len, 0)
#else
#ifdef INTRANET_SERVER_TEST
    #define	LOGE(fmt, ...)  printf(fmt, ##__VA_ARGS__)
    #define	LOGW(fmt, ...)  printf(fmt, ##__VA_ARGS__)
    #define	LOGD(fmt, ...)  printf(fmt, ##__VA_ARGS__)
    #define	LOGI(fmt, ...)  printf(fmt, ##__VA_ARGS__)
    #define PRINT_HEX_ASCII(buf, len)
    #define PRINT_HEX(buf, len)
#else
    #define	LOGE(fmt, ...)
    #define	LOGW(fmt, ...)
    #define	LOGD(fmt, ...)
    #define	LOGI(fmt, ...)
    #define PRINT_HEX_ASCII(buf, len)
    #define PRINT_HEX(buf, len)
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif

