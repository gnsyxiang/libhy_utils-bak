/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    utils_cjson.c
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
#include "utils_cjson.h"

#include "hal/hal_type.h"
#include "hal/hal_string.h"
#include "hal/hal_mem.h"
#include "hal/hal_log.h"

static hal_int32_t cjson_hook_flag = 0;

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
    hal_char_t *field_fmt = Hal_calloc(1, ALIGN4(field_len) + ALIGN4(1));
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
    if (0 == cjson_hook_flag) {
        cjson_hook_flag = 1;

        cJSON_Hooks hooks;
        hooks.malloc_fn = Hal_malloc;
        hooks.free_fn   = Hal_free;
        cJSON_InitHooks(&hooks);
    }
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

void *_get_item_com(cJSON *root, hal_char_t *field, hal_uint32_t field_len)
{
    if (NULL == root || NULL == field || field_len <= 0) {
        HalLogE("the param is NULL \n");
        return NULL;
    }

    cJSON *item = _get_item(root, field, field_len);
    if (NULL == item) {
        HalLogE("get item is NULL \n");
        return  NULL;
    }

    return item;
}

#define _get_item_com_macro(root, field, field_len, val_type)   \
    ({                                                          \
        cJSON *item = _get_item_com(root, field, field_len);    \
        if (NULL == item || val_type != item->type) {           \
            HalLogE("the cJSON type error \n");                 \
            return  HAL_INVALID_PARAM_ERR;                      \
        }                                                       \
        item;                                                   \
    })

hal_int32_t UtilsJsonGetInt(cJSON *root, hal_int32_t *val, 
                            hal_char_t *field, hal_uint32_t field_len)
{
    cJSON *item = _get_item_com_macro(root, field, field_len, cJSON_Number);

    if (NULL != val) {
        *val = item->valueint;
    }

    return HAL_NO_ERR;
}

hal_int32_t UtilsJsonGetDouble(cJSON *root, hal_double_t *val, 
                               hal_char_t *field, hal_uint32_t field_len)
{
    cJSON *item = _get_item_com_macro(root, field, field_len, cJSON_Number);

    if (NULL != val) {
        *val = item->valuedouble;
    }

    return HAL_NO_ERR;
}

hal_int32_t UtilsJsonGetString(cJSON *root, hal_char_t *val, 
                               hal_char_t *field, hal_uint32_t field_len)
{
    cJSON *item = _get_item_com_macro(root, field, field_len, cJSON_String);

    if (NULL != val) {
        Hal_strncpy(val, item->valuestring, Hal_strlen(item->valuestring));
    }

    return HAL_NO_ERR;
}

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

    if (js && cJSON_Number == js->type) {
        return js->valueint;
    }
    return -1;
}

#define comac_args_seqs() \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
    9,   8,  7,  6,  5,  4,  3,  2,  1,  0

#define comac_arg_n( \
        _0,   _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, \
        _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
        _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
        _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, \
        \
        _N, ...) _N

#define comac_get_args_cnt(...) comac_arg_n(__VA_ARGS__)

/**
 *  comac_argc: calculate macro args' count, maxim to 36
 */
#define comac_argc(...) comac_get_args_cnt(0, ##__VA_ARGS__, comac_args_seqs())

#define cJSON_GetItemIntValue(error_val, json, x...) cJSON_GetItemIntValue_va(json, error_val, comac_argc(x), x)

hal_char_t text_json[]="{\n\
	\"image\": {\n\
		\"width\":  800,\n\
		\"height\": 600,\n\
		\"title\":  \"View from 15th Floor\",\n\
		\"thumbnail\": {\n\
			\"url\":    \"http:/*www.example.com/image/481989943\",\n\
			\"height\": 125,\n\
			\"width\":  \"100\"\n\
		},\n\
		\"int\": [1, 2, 3, 4],\n\
		\"double\": [1.1, 0.2, 0.3],\n\
		\"string\": [\"haha\", \"heihei\"]\n\
	}\n\
}";

void json_test(void)
{
    cJSON *root = cJSON_Parse(text_json);

    int height = cJSON_GetItemIntValue(-1, root, "image", "thumbnail", "height");
    printf("Height: %d \n", height);

    cJSON_Delete(root);
}

#endif

