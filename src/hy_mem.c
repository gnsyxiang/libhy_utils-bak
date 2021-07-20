/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    hy_mem.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    22/02 2020 16:42
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        22/02 2020      create the file
 * 
 *     last modified: 22/02 2020 16:42
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hy_mem.h"

#include "hy_assert.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

void *HyMalloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr) {
        LOGE("malloc faild \n");
        return NULL;
    }
    memset(ptr, '\0', size);

    return ptr;
}

void HyFree(void **pptr)
{
    if (pptr && *pptr) {
        free(*pptr);
        *pptr = NULL;
    }
}

void *HyCalloc(size_t nmemb, size_t size)
{
    return calloc(nmemb, size);
}

void *HyRealloc(void *ptr, size_t size)
{
    HY_ASSERT_NULL_RET_VAL(!ptr, NULL);
    return realloc(ptr, size);
}

