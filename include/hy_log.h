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

#define USE_DEBUG

#ifdef USE_DEBUG
enum {
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_MAX,
};

// 下面函数和宏为内部用函数，最好不要在外面使用
int32_t HyLogWrite(uint8_t level, const char *tags, const char *func, uint32_t line, char *fmt, ...);

#define LOG(no_debug, level, fmt, ...)                                  \
    do {                                                                \
        if (no_debug) {                                                 \
            HyLogWrite(level, LOG_CATEGORY_TAG, __func__, __LINE__,     \
                    (char *)fmt, ##__VA_ARGS__);                        \
        }                                                               \
    } while (0)
#endif

/*
 * API
 */
int32_t HyLogInit(char *buf, uint32_t len, uint8_t level);
void HyLogDestory(void);

#ifdef USE_DEBUG
    #define	LOGE(fmt, ...)  LOG(ALONE_DEBUG, LOG_LEVEL_ERROR,    fmt, ##__VA_ARGS__)
    #define	LOGW(fmt, ...)  LOG(ALONE_DEBUG, LOG_LEVEL_WARNING,  fmt, ##__VA_ARGS__)
    #define	LOGD(fmt, ...)  LOG(ALONE_DEBUG, LOG_LEVEL_DEBUG,    fmt, ##__VA_ARGS__)
    #define	LOGI(fmt, ...)  LOG(ALONE_DEBUG, LOG_LEVEL_INFO,     fmt, ##__VA_ARGS__)
#else
#ifdef INTRANET_SERVER_TEST
    #define	LOGE(fmt, ...)  printf(fmt, ##__VA_ARGS__)
    #define	LOGW(fmt, ...)  printf(fmt, ##__VA_ARGS__)
    #define	LOGD(fmt, ...)  printf(fmt, ##__VA_ARGS__)
    #define	LOGI(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#else
    #define	LOGE(fmt, ...)
    #define	LOGW(fmt, ...)
    #define	LOGD(fmt, ...)
    #define	LOGI(fmt, ...)
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif

