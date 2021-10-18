/**
 *
 * Release under GPL-3.0.
 * 
 * @file    hy_string.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    21/02 2020 16:46
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        21/02 2020      create the file
 * 
 *     last modified: 21/02 2020 16:46
 */
#ifndef __LIBHY_UTILS_INCLUDE_HY_STRING_H_
#define __LIBHY_UTILS_INCLUDE_HY_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

#define HY_MEMCMP(dst, src, size)               \
    ({                                          \
     int32_t val = memcpy(dst, src, size);      \
     val;                                       \
     })

#define HY_MEMSET(ptr)                          \
    do {                                        \
        memset((ptr), '\0', sizeof(*(ptr)));    \
    } while (0)

#define HY_MEMCPY(dst, src)                     \
    do {                                        \
        memcpy((dst), (src), sizeof(*(dst)));   \
    } while (0)

#define HY_STRNCPY(dst, src, max_len)           \
    do {                                        \
        memset(dst, '\0', max_len);             \
        strncpy(dst, src, max_len - 1);         \
    } while (0)

/**
 * @brief 从右边开始拷贝字符，直到指定字符停止，不拷贝指定字符
 *
 * @param src 原始字符串
 * @param dst 目的字符串
 * @param len 目的字符串长度
 * @param ch 指定分界字符
 */
void HyStrCopyRight(const char *src, char *dst, size_t len, char ch);

/**
 * @brief 从右边开始拷贝字符，直到指定字符停止，不拷贝指定字符
 *
 * @param src 原始字符串
 * @param dst 目的字符串
 * @param len 目的字符串长度
 * @param ch 指定第一个分界字符
 * @param ch2 指定第二个分界字符
 */
void HyStrCopyRight2(const char *src, char *dst, size_t len, char ch, char ch2);

#ifdef __cplusplus
}
#endif

#endif

