/**
 *
 * Release under GPL-3.0.
 * 
 * @file    utils_json.h
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
#ifndef __UTILS_INCLUDE_UTILS_JSON_H_
#define __UTILS_INCLUDE_UTILS_JSON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hal/hal_type.h"
#include "cJSON.h"

hal_int32_t UtilsJsonGetInt(cJSON *root, hal_int32_t *val, 
                            hal_char_t *field, hal_uint32_t field_len);

hal_int32_t UtilsJsonGetDouble(cJSON *root, hal_double_t *val, 
                               hal_char_t *field, hal_uint32_t field_len);

hal_int32_t UtilsJsonGetString(cJSON *root, hal_char_t *val, 
                               hal_char_t *field, hal_uint32_t field_len);
#ifdef __cplusplus
}
#endif

#endif // __UTILS_INCLUDE_UTILS_JSON_H_

