/**
 *
 * Release under GPL-3.0.
 * 
 * @file    utils_md5sum.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    14/04 2020 14:46
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        14/04 2020      create the file
 * 
 *     last modified: 14/04 2020 14:46
 */
#ifndef __LIBUTILS_INCLUDE_UTILS_MD5SUM_H_
#define __LIBUTILS_INCLUDE_UTILS_MD5SUM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hal/hal_type.h"

#define MD5SUM_LEN  (16)

void UtilsMd5sum(const hal_uint8_t *msg, size_t msg_len, hal_uint8_t digest[MD5SUM_LEN]);

#ifdef __cplusplus
}
#endif

#endif // __LIBUTILS_INCLUDE_UTILS_MD5SUM_H_

