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
#include "hy_assert.h"
#include "hy_mem.h"

#define ALONE_DEBUG 1

static inline void _mem_swap(void *dst, void *src,
        void *tmp, hy_u32_t item_len)
{
    memcpy(tmp, dst, item_len);
    memcpy(dst, src, item_len);
    memcpy(src, tmp, item_len);
}

void HySortBubble(void *array, hy_u32_t len,
        hy_u32_t item_len, HySortSwapCb_t swap_cb)
{
    HY_ASSERT_NULL_RET(!array || !swap_cb);

    if (0 == len || 0 == item_len) {
        LOGE("the len or item_len is zero \n");
        return;
    }

    char *tmp = HY_MALLOC_RET(item_len);

    for (hy_u32_t i = 0; i < len; i++) {
        for (hy_u32_t j = 0; j + 1 < len - i; j++) {
            void *src = array + item_len * j;
            void *dst = array + item_len * (j + 1);
            if (swap_cb(src, dst) > 0) {
                _mem_swap(dst, src, tmp, item_len);
            }
        }
    }

    HY_FREE(&tmp);
}

static hy_u32_t _partition(void *array, hy_s32_t low, hy_s32_t high,
        hy_u32_t item_len, HySortSwapCb_t swap_cb)
{
#define _ARRAY_LOW (array + low * item_len)
#define _ARRAY_HIGHT (array + high * item_len)

    void *tmp = HY_MALLOC_RET_VAL(item_len, -1);
    void *swap_tmp = HY_MALLOC_RET_VAL(item_len, -1);

    memcpy(tmp, _ARRAY_LOW, item_len);

    while (low < high) {
        while (low < high && swap_cb(_ARRAY_HIGHT, tmp) >= 0) {
            high--;
        }
        _mem_swap(_ARRAY_LOW, _ARRAY_HIGHT, swap_tmp, item_len);

        while (low < high && swap_cb(_ARRAY_LOW, tmp) <= 0) {
            low++;
        }
        _mem_swap(_ARRAY_HIGHT, _ARRAY_LOW, swap_tmp, item_len);
    }

    memcpy(_ARRAY_LOW, tmp, item_len);

    HY_FREE(&tmp);
    HY_FREE(&swap_tmp);

    return low;
}

void HySortQuick(void *array, hy_s32_t low, hy_s32_t high,
        hy_u32_t item_len, HySortSwapCb_t swap_cb)
{
    HY_ASSERT_NULL_RET(!array || !swap_cb);

    if (0 == item_len) {
        LOGE("the item_len is zero \n");
        return;
    }

    if (low < high) {
        hy_u32_t positiion = _partition(array, low, high, item_len, swap_cb);

        // note: 不能用hy_u32_t类型，否则会出现负数导致程序出错
        HySortQuick(array, low, positiion - 1, item_len, swap_cb);
        HySortQuick(array, positiion + 1, high, item_len, swap_cb);
    }
}

