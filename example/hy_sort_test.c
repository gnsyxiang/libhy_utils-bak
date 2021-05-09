/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_sort_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    09/05 2021 17:35
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        09/05 2021      create the file
 * 
 *     last modified: 09/05 2021 17:35
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hy_sort.h"
#include "hy_utils.h"
#include "hy_log.h"

#define ALONE_DEBUG 1
#define LOG_CATEGORY_TAG "main"

static hy_int32_t _swap_cb(void *src, void *dst)
{
    int *a = src;
    int *b = dst;

    return (*a > *b);
}

static void _dump(hy_int32_t *a, hy_uint32_t len)
{
    for (hy_uint32_t i = 0; i < len; ++i) {
        printf("%d ", *(a + i));
    }
    printf("\n");
}

int main(int argc, char const* argv[])
{
    HyLogCreate(HY_LOG_LEVEL_INFO, 512);

    int a[] = {3, 4, 1, 5, 8, 0, 9};
    int len = HY_ARRAY_CNT(a);

    _dump(a, len);

    HySortBubble(a, len, sizeof(a[0]),  _swap_cb);

    _dump(a, len);

    HyLogDestroy();

    return 0;
}
