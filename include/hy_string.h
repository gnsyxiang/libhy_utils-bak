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
#include <assert.h>
#include <string.h>

#define HY_STRING_BUF_MAX_LEN_4             (4)
#define HY_STRING_BUF_MAX_LEN_8             (8)
#define HY_STRING_BUF_MAX_LEN_16            (16)
#define HY_STRING_BUF_MAX_LEN_32            (32)
#define HY_STRING_BUF_MAX_LEN_64            (64)
#define HY_STRING_BUF_MAX_LEN_128           (128)
#define HY_STRING_BUF_MAX_LEN_256           (256)
#define HY_STRING_BUF_MAX_LEN_512           (512)

#define HY_MEMCMP(dst, src, size) memcmp(dst, src, size)

#define HY_MEMSET(ptr, len)                 \
    do {                                    \
        memset((ptr), '\0', len);           \
    } while (0)

#define HY_MEMCPY(dst, src, len)            \
    do {                                    \
        memcpy(dst, src, len);              \
    } while (0)

#define HY_STRNCPY(dst, src, len)           \
    do {                                    \
        assert(sizeof(dst) > len);          \
        memset(dst, '\0', sizeof(dst));     \
        strncpy(dst, src, len);             \
    } while (0)

#define HY_STRLEN(str) strlen(str)

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

