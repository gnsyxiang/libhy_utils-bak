/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_sort.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    09/05 2021 17:01
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        09/05 2021      create the file
 * 
 *     last modified: 09/05 2021 17:01
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hy_sort.h"
#include "hy_log.h"

#define ALONE_DEBUG 1
#define LOG_CATEGORY_TAG "hy_sort"

void HySortBubble(void *array, hy_uint32_t len,
        hy_uint32_t item_len, HySortSwapCb_t swap_cb)
{
    if (!array || !swap_cb) {
        LOGE("the array or swap_cb is NULL \n");
        return;
    }
    if (0 == len || 0 == item_len) {
        LOGE("the len or item_len is zero \n");
        return;
    }

    char *tmp = calloc(1, item_len);
    if (!tmp) {
        LOGE("calloc is faild \n");
        return;
    }

    for (hy_uint32_t i = 0; i < len; i++) {
        for (hy_uint32_t j = 0; j + 1 < len - i; j++) {
            void *src = array + item_len * j;
            void *dst = array + item_len * (j + 1);
            if (swap_cb(src, dst) > 0) {
                memcpy(tmp, dst, item_len);
                memcpy(dst, src, item_len);
                memcpy(src, tmp, item_len);
            }
        }
    }

    free(tmp);
}

