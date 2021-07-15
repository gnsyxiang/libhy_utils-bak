/**
 *
 * Release under GPL-3.0.
 * 
 * @file    hy_mem.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    20/01 2020 17:50
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        20/01 2020      create the file
 * 
 *     last modified: 20/01 2020 17:50
 */
#ifndef __LIBHY_UTILS_INCLUDE_HY_MEM_H_
#define __LIBHY_UTILS_INCLUDE_HY_MEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#define HY_MALLOC_RETURN_VAL(size, ret) \
    ({                                  \
        void *ptr = NULL;               \
        ptr = malloc(size);             \
        if (!ptr) {                     \
            LOGE("malloc faild \n");    \
            return ret;                 \
        } else {                        \
            memset(ptr, '\0', size);    \
        }                               \
        ptr;                            \
     })

#define HY_MALLOC_RETURN(size)          \
    ({                                  \
        void *ptr = NULL;               \
        ptr = malloc(size);             \
        if (!ptr) {                     \
            LOGE("malloc faild \n");    \
            return;                     \
        } else {                        \
            memset(ptr, '\0', size);    \
        }                               \
        ptr;                            \
     })

#define HY_MALLOC_BREAK(size)           \
    ({                                  \
        void *ptr = NULL;               \
        ptr = malloc(size);             \
        if (!ptr) {                     \
            LOGE("malloc faild \n");    \
            break;                      \
        } else {                        \
            memset(ptr, '\0', size);    \
        }                               \
        ptr;                            \
     })

#define HY_FREE(pptr)                   \
    do {                                \
        if (pptr && *pptr) {            \
            free(*pptr);                \
            *pptr = NULL;               \
        }                               \
    } while (0)

void *HyMalloc(size_t size);

void HyFree(void **pptr);

void *HyCalloc(size_t nmemb, size_t size);

void *HyRealloc(void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif

