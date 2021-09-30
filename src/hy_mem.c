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

#include "hy_mem.h"

#include "hy_log.h"

#define ALONE_DEBUG 1

void *HyMalloc(size_t size)
{
    LOGE("HyMalloc error \n");

    return NULL;
}

void HyFree(void **pptr)
{
    LOGE("HyFree error \n");
}

void *HyCalloc(size_t nmemb, size_t size)
{
    LOGE("HyCalloc error \n");

    return NULL;
}

void *HyRealloc(void *ptr, size_t size)
{
    LOGE("HyRealloc error \n");

    return NULL;
}

