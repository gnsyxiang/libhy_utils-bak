/**
 *
 * Release under GPL-3.0.
 * 
 * @file    hy_cjson_file.h
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
#ifndef __LIBUTILS_INCLUDE_HY_CJSON_FILE_H_
#define __LIBUTILS_INCLUDE_HY_CJSON_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "cjson/cJSON.h"

cJSON *HyJsonFileGetRoot(char *file);
void HyJsonFilePutRoot(cJSON **root);

int32_t HyJsonFileGetInt(char *file, int32_t *val, char *field, uint32_t field_len);
int32_t HyJsonFileGetDouble(char *file, double *val, char *field, uint32_t field_len);
int32_t HyJsonFileGetString(char *file, char *val, char *field, uint32_t field_len);

#ifdef __cplusplus
}
#endif

#endif // __LIBUTILS_INCLUDE_UTILS_CJSON_FILE_H_

