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

#define HY_MEMSET(ptr)                          \
    do {                                        \
        memset((ptr), '\0', sizeof(*(ptr)));    \
    } while (0)

#define HY_MEMCPY(dst, src)                     \
    do {                                        \
        memcpy((dst), src, sizeof(*(dst)));     \
    } while (0)

#define HY_STRNCPY(dst, src, max_len)           \
    do {                                        \
        memset(dst, '\0', max_len);             \
        size_t len = strlen(src);               \
        if (len >= max_len) {                   \
            len = max_len - 1;                  \
        }                                       \
        strncpy(dst, src, len);                 \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif

