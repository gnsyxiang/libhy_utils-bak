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

#include "cJSON.h"
#include "hal/hal_config.h"

void *UtilsJsonGetInt(cJSON *json, hal_char_t *field, hal_int32_t val);
// void *UtilsJsonGetDouble(cJSON *json, hal_char_t *field, hal_double_t val);
// void *UtilsJsonGetString(cJSON *json, hal_char_t *field, hal_char_t val);

#ifdef __cplusplus
}
#endif

#endif // __UTILS_INCLUDE_UTILS_JSON_H_

