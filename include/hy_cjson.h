/**
 *
 * Release under GPL-3.0.
 * 
 * @file    hy_cjson.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    03/04 2020 22:31
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        03/04 2020      create the file
 * 
 *     last modified: 03/04 2020 22:31
 */
#ifndef __LIBUTILS_INCLUDE_HY_CJSON_H_
#define __LIBUTILS_INCLUDE_HY_CJSON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "cjson/cJSON.h"

int32_t HyJsonGetInt(cJSON *root, int32_t *val, char *field, uint32_t field_len);
int32_t HyJsonGetDouble(cJSON *root, double *val, char *field, uint32_t field_len);
int32_t HyJsonGetString(cJSON *root, char *val, char *field, uint32_t field_len);

#ifdef __cplusplus
}
#endif

#endif // __LIBUTILS_INCLUDE_UTILS_CJSON_H_

