/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_string.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    20/08 2021 17:11
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        20/08 2021      create the file
 * 
 *     last modified: 20/08 2021 17:11
 */
#include <stdio.h>
#include <string.h>

#include "hy_assert.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

void HyStrCopyRight(const char *src, char *dst, size_t len, char ch)
{
    HY_ASSERT_VAL_RET(!src || !dst);

    memset(dst, '\0', len);

    size_t src_len = strlen(src);
    size_t cnt = 0;
    const char *str = src + src_len - 1;
    while (*str && cnt < src_len) {
        if (*str == ch) {
            break;
        }
        str--;
        cnt++;
    }

    if (cnt >= len) {
        cnt = len - 1;
    }

    memcpy(dst, str + 1, cnt);
}

void HyStrCopyRight2(const char *src, char *dst, size_t len, char ch, char ch2)
{
    HY_ASSERT_VAL_RET(!src || !dst);

    memset(dst, '\0', len);

    size_t src_len = strlen(src);
    size_t cnt = 0;
    const char *str = src + src_len - 1;
    while (*str && cnt < src_len) {
        if (*str == ch || *str == ch2) {
            break;
        }
        str--;
        cnt++;
    }

    if (cnt >= len) {
        cnt = len - 1;
    }

    memcpy(dst, str + 1, cnt);
}

