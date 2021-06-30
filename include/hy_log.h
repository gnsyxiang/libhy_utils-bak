/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_log.h
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
#ifndef __LIBHY_UTILS_INCLUDE_HY_LOG_H_
#define __LIBHY_UTILS_INCLUDE_HY_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief 打印等级定义
 */
typedef enum {
    HY_LOG_LEVEL_OFF        = 60000,
    HY_LOG_LEVEL_FATAL      = 50000,
    HY_LOG_LEVEL_ERROR      = 40000,
    HY_LOG_LEVEL_WARN       = 30000,
    HY_LOG_LEVEL_INFO       = 20000,
    HY_LOG_LEVEL_DEBUG      = 10000,
    HY_LOG_LEVEL_TRACE      = 0,
    HY_LOG_LEVEL_ALL        = HY_LOG_LEVEL_TRACE,
    HY_LOG_LEVEL_NOT_SET    = -1,
} HyLogLevel_t;

/**
 * @brief 输出log宏转义
 *
 * note: 宏为内部用，最好不要在外面使用
 */
#define LOG(no_debug, level, fmt, ...)                                  \
    do {                                                                \
        if (no_debug) {                                                 \
            HyLogWrite(level, __FILE__, __func__, __LINE__,             \
                    (char *)fmt, ##__VA_ARGS__);                        \
        }                                                               \
    } while (0)

void HyLogWrite(int level, const char *file, 
        const char *func, uint32_t line, char *fmt, ...);

void HyPrintHex(const char *name, uint16_t line,
        const char *buf, int32_t len, int8_t flag);

/**
 * @brief 创建log打印系统
 *
 * @param level: 打印等级
 * @param buf_len: 一行log的buf长度
 * @param config_file: 配置文件
 */
void HyLogCreate(int32_t level, uint32_t buf_len, const char *config_file);

/**
 * @brief 销毁log系统
 */
void HyLogDestroy(void);

/**
 * @brief 输出对应的log等级函数
 */
#define LOGF(fmt, ...) LOG(ALONE_DEBUG, HY_LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) LOG(ALONE_DEBUG, HY_LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) LOG(ALONE_DEBUG, HY_LOG_LEVEL_WARN,  fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) LOG(ALONE_DEBUG, HY_LOG_LEVEL_INFO,  fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...) LOG(ALONE_DEBUG, HY_LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define LOGT(fmt, ...) LOG(ALONE_DEBUG, HY_LOG_LEVEL_TRACE, fmt, ##__VA_ARGS__)

#define PRINT_HEX_ASCII(buf, len) HyPrintHex(__func__, __LINE__, buf, len, 1)
#define PRINT_HEX(buf, len) HyPrintHex(__func__, __LINE__, buf, len, 0)

#ifdef __cplusplus
}
#endif

#endif

