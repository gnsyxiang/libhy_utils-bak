/**
 *
 * Release under GPL-3.0.
 * 
 * @file    utils_hash.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    15/04 2020 22:44
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        15/04 2020      create the file
 * 
 *     last modified: 15/04 2020 22:44
 */
#ifndef __LIBUTILS_INCLUDE_UTILS_HASH_H_
#define __LIBUTILS_INCLUDE_UTILS_HASH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hal/hal_type.h"

typedef struct {
    hal_char_t  *key;
    hal_char_t  *val;
} HashItem_t;
#define HASH_ITEM_LEN (sizeof(HashItem_t))

typedef struct {
    hal_uint32_t        bucket_max_len;
} HashConfig_t;
#define HASH_CONFIG_LEN (sizeof(HashConfig_t))

typedef void *HashHandle_t;

hal_uint32_t UtilsHashGenerate(const hal_char_t *key);

HashHandle_t UtilsHashCreate(HashConfig_t *config);
void UtilsHashDestroy(HashHandle_t handle);
void UtilsHashDump(HashHandle_t handle);

hal_int32_t UtilsHashItemAdd(HashHandle_t handle, HashItem_t *hash_item);
hal_int32_t UtilsHashItemDel(HashHandle_t handle, HashItem_t *hash_item);

#ifdef __cplusplus
}
#endif

#endif // __LIBUTILS_INCLUDE_UTILS_HASH_H_

