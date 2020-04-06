/**
 *
 * Release under GPL-3.0.
 * 
 * @file    utils_cjson_file.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/04 2020 18:16
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/04 2020      create the file
 * 
 *     last modified: 06/04 2020 18:16
 */
#ifndef __UTILS_INCLUDE_UTILS_CJSON_FILE_H_
#define __UTILS_INCLUDE_UTILS_CJSON_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hal/hal_type.h"
#include "cJSON.h"

cJSON *UtilsJsonFileGetRoot(hal_char_t *file);
void UtilsJsonFilePutRoot(cJSON **root);

hal_int32_t UtilsJsonFileGetInt(hal_char_t *file, hal_int32_t *val, 
                                hal_char_t *field, hal_uint32_t field_len);

// hal_int32_t UtilsJsonFileGetDouble(hal_char_t *file, hal_double_t *val,
                                   // hal_char_t *field, hal_uint32_t field_len);
//
// hal_int32_t UtilsJsonFileGetString(hal_char_t *file, hal_char_t *val,
                                   // hal_char_t *field, hal_uint32_t field_len);

#ifdef __cplusplus
}
#endif

#endif // __UTILS_INCLUDE_UTILS_CJSON_FILE_H_

