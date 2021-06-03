/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_log4cplus_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    01/06 2021 14:48
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        01/06 2021      create the file
 * 
 *     last modified: 01/06 2021 14:48
 */
#ifndef __LIBHY_UTILS_INCLUDE_LOG4CPLUS_WRAPPER_H_
#define __LIBHY_UTILS_INCLUDE_LOG4CPLUS_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief 输出等级
 *
 * note: 直接拷贝log4cpuls
 */
enum {
    OFF_LOG_LEVEL     = 60000,
    FATAL_LOG_LEVEL   = 50000,
    ERROR_LOG_LEVEL   = 40000,
    WARN_LOG_LEVEL    = 30000,
    INFO_LOG_LEVEL    = 20000,
    DEBUG_LOG_LEVEL   = 10000,
    TRACE_LOG_LEVEL   = 0,
    ALL_LOG_LEVEL     = TRACE_LOG_LEVEL,
    NOT_SET_LOG_LEVEL = -1,
};

/**
 * @brief 输出log宏转义
 *
 * note: 宏为内部用，最好不要在外面使用
 */
#define LOG(no_debug, level, fmt, ...)                                  \
    do {                                                                \
        if (no_debug) {                                                 \
            HyLog4cplusWrite(level, __FILE__, __func__, __LINE__,       \
                    (char *)fmt, ##__VA_ARGS__);                        \
        }                                                               \
    } while (0)

void HyLog4cplusWrite(int level, const char *file, 
        const char *func, uint32_t line, char *fmt, ...);

/**
 * @brief 创建log打印系统
 *
 * @param buf_len: 一行log的buf长度
 * @param config_file: log4cplus的配置文件
 */
void HyLog4cplusCreate(uint32_t buf_len, const char *config_file);


/**
 * @brief 销毁log系统
 */
void HyLog4cplusDestroy(void);

/**
 * @brief 输出对应的log等级函数
 */
#define LOGF(fmt, ...) LOG(ALONE_DEBUG, FATAL_LOG_LEVEL, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) LOG(ALONE_DEBUG, ERROR_LOG_LEVEL, fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) LOG(ALONE_DEBUG, WARN_LOG_LEVEL,  fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) LOG(ALONE_DEBUG, INFO_LOG_LEVEL,  fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...) LOG(ALONE_DEBUG, DEBUG_LOG_LEVEL, fmt, ##__VA_ARGS__)
#define LOGT(fmt, ...) LOG(ALONE_DEBUG, TRACE_LOG_LEVEL, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif

