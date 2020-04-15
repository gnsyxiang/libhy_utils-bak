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

hal_uint32_t UtilsHash(const hal_char_t *key);

#ifdef __cplusplus
}
#endif

#endif // __LIBUTILS_INCLUDE_UTILS_HASH_H_

