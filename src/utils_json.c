/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    utils_json.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    03/04 2020 22:32
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        03/04 2020      create the file
 * 
 *     last modified: 03/04 2020 22:32
 */
#include "utils_json.h"

#include "hal/hal_type.h"
#include "hal/hal_string.h"
#include "hal/hal_mem.h"
#include "hal/hal_log.h"

static hal_int32_t _get_index(hal_char_t *fmt)
{
    hal_int32_t index;
    hal_char_t *start = Hal_strstr(fmt, "[");
    if (NULL != start) {
        hal_char_t *end = Hal_strstr(fmt, "]");
        if (NULL != end) {
            *start = *end = '\0';
            start++;
            sscanf(start, "%d", &index);
            if (index >= 0) {
                return index;
            }
        }
    }
    return -1;
}

static hal_char_t *_field_fmt_init(const hal_char_t *field, hal_uint32_t field_len)
{
    hal_char_t *field_fmt = Hal_calloc(1, field_len);
    if (NULL == field_fmt) {
        HalLogE("calloc failed \n");
        return NULL;
    }
    Hal_strncpy(field_fmt, field, field_len);

    return field_fmt;
}

static inline void _field_fmt_final(hal_char_t **field_tmp)
{
    hal_char_t *field = *field_tmp;
    if (NULL != field) {
        Hal_free(field);
        *field_tmp = NULL;
    }
}

static cJSON *_get_item(cJSON *root, const char *field, hal_uint32_t field_len)
{
    hal_char_t *field_fmt = _field_fmt_init(field, field_len);
    hal_char_t *field_fmt_p = field_fmt;

    for (hal_uint32_t i = 0; i < field_len; i++) {
        if ('.' == field_fmt[i]) {
            field_fmt[i] = '\0';
        }
    }

    hal_uint32_t offset = 0;
    hal_int32_t index = 0;
    cJSON *child = NULL;
    cJSON *parent = root;
    hal_int32_t field_len_tmp = field_len;

    while (field_len_tmp > 0) {
        offset = Hal_strlen(field_fmt) + 1;
        index = _get_index(field_fmt);
        child = cJSON_GetObjectItem(parent, field_fmt);
        if (NULL == child) {
            goto L_GET_ITEM_1;
        }
        if (index >= 0) {
            child = cJSON_GetArrayItem(child, index);
            if (NULL == child) {
                goto L_GET_ITEM_1;
            }
        }
        parent         = child;
        field_fmt     += offset;
        field_len_tmp -= offset;
    }

L_GET_ITEM_1:
    _field_fmt_final(&field_fmt_p);
    return child;
}

hal_int32_t UtilsJsonGetInt(cJSON *root, hal_char_t *field, hal_uint32_t field_len, hal_int32_t *val)
{
    if (NULL == root || NULL == val || NULL == field || field_len <= 0) {
        HalLogE("the param is NULL \n");
        return  HAL_INVALID_PARAM_ERR;
    }

    cJSON *item = _get_item(root, field, field_len);
    if (NULL == item) {
        HalLogE("get item is NULL \n");
        return  HAL_INVALID_PARAM_ERR;
    }

    if (cJSON_Number != item->type) {
        HalLogE("the cJSON type error \n");
        return  HAL_INVALID_PARAM_ERR;
    }

    *val = item->valueint;

    return HAL_NO_ERR;
}

// void *UtilsJsonGetDouble(cJSON *json, hal_char_t *field, hal_double_t val)
// {
// return NULL;
// }
//
// void *UtilsJsonGetString(cJSON *json, hal_char_t *field, hal_char_t val)
// {
// return NULL;
// }

#if 0
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

static cJSON *cJSON_GetItem_valist(cJSON *json, int n, va_list list)
{
    int i;
    cJSON *js = json;

    for (i = 0; i < n; i++) {
        int index;
        const char *str;

        if (js->type == cJSON_Array) {
            index = va_arg(list, int);
            js = cJSON_GetArrayItem(js, index);
        } else {
            str = va_arg(list, const char *);
            js = cJSON_GetObjectItem(js, str);
        }

        if (js == NULL)
            break;
    }

    return js;
}

int cJSON_GetItemIntValue_va(cJSON *json, int n, ...)
{
    cJSON *js;
    va_list list;

    va_start(list, n);
    js = cJSON_GetItem_valist(json, n, list);
    va_end(list);

    return js->valueint;
}
#endif

