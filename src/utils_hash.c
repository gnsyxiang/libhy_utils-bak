/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    utils_hash.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    15/04 2020 22:43
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        15/04 2020      create the file
 * 
 *     last modified: 15/04 2020 22:43
 */
#include "hal/hal_log.h"

#include "utils_hash.h"

hal_uint32_t UtilsHash(const hal_char_t *key)
{
    if (NULL == key) {
        HalLogE("hash key input is NULL \n");
        return 0;
    }

    hal_uint32_t seed = 31; // 31 131 1313 13131 131313 etc..
    hal_uint32_t hash = 0;

    while (*key) {
        hash = hash * seed + (*key++);
    }

    return hash;
}

