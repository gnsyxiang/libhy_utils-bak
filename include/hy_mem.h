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

/* 
 * BYTE_ALIGN:  字节对齐
 * ALIGN4:      4字节对齐
 * ALIGN2:      2字节对齐
 * ALIGN4_UP:   绝对字节对齐(原来已经事4字节对齐，使用后再增加4个字节)
 */
#define BYTE_ALIGN(len, align)  (((len) + (align) - 1) & ~((align) - 1))
#define ALIGN4(len)             BYTE_ALIGN(len, 4)
#define ALIGN2(len)             BYTE_ALIGN(len, 2)
#define ALIGN4_UP(len)          (BYTE_ALIGN(len, 4) + ALIGN4(1))

#define HY_MALLOC_RET_VAL(type, size, ret)  \
    ({                                      \
        void *ptr = malloc((size));         \
        if (!ptr) {                         \
            LOGE("malloc faild \n");        \
            return ret;                     \
        } else {                            \
            memset(ptr, '\0', (size));      \
        }                                   \
        (type)ptr;                          \
     })

#define HY_MALLOC_RET(type, size)           \
    ({                                      \
        void *ptr = malloc((size));         \
        if (!ptr) {                         \
            LOGE("malloc faild \n");        \
            return;                         \
        } else {                            \
            memset(ptr, '\0', (size));      \
        }                                   \
        (type)ptr;                          \
     })

#define HY_MALLOC_BREAK(type, size)         \
    ({                                      \
        void *ptr = malloc((size));         \
        if (!ptr) {                         \
            LOGE("malloc faild \n");        \
            break;                          \
        } else {                            \
            memset(ptr, '\0', (size));      \
        }                                   \
        (type)ptr;                          \
     })

#define HY_FREE_P(ptr)                      \
    do {                                    \
        if (ptr) {                          \
            free(ptr);                      \
            ptr = NULL;                     \
        }                                   \
    } while (0)

#define HY_FREE_PP(pptr)                    \
    do {                                    \
        if (pptr && *pptr) {                \
            free(*pptr);                    \
            *pptr = NULL;                   \
        }                                   \
    } while (0)

/**
 * @brief
 *
 * @param size
 *
 * @return 
 */
void *HyMalloc(size_t size);

/**
 * @brief 
 *
 * @param pptr
 */
void HyFree(void **pptr);

/**
 * @brief 
 *
 * @param nmemb
 * @param size
 *
 * @return 
 */
void *HyCalloc(size_t nmemb, size_t size);

/**
 * @brief 
 *
 * @param ptr
 * @param size
 *
 * @return 
 */
void *HyRealloc(void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif

