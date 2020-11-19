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

/**
 * @brief 打印等级定义
 */
#define LOG_LEVEL_ERROR         (1)
#define LOG_LEVEL_WARNING       (2)
#define LOG_LEVEL_DEBUG         (3)
#define LOG_LEVEL_INFO          (4)
#define LOG_LEVEL_MAX           (5)

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
#endif

/**
 * @brief 初始化log打印系统
 *
 * @param buf: 打印buf空间
 * @param len: 打印buf空间的总长度
 * @param level: 打印等级，详见打印等级定义
 *
 * @return 成功返回0
 *
 * note: buf空间需要使用者维护的原因:
 * 1, 使用者知道该空间何时创建，何时释放
 * 2，对于单片机系统没有堆的概念，不能在内部使用malloc创建空间
 */
int32_t HyLogInit(char *buf, uint32_t len, uint8_t level);

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

