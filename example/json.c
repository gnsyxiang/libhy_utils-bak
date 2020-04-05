/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    json.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    03/04 2020 23:11
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        03/04 2020      create the file
 * 
 *     last modified: 03/04 2020 23:11
 */
#include "utils_json.h"

#include "hal/hal_type.h"
#include "hal/hal_string.h"
#include "hal/hal_log.h"

hal_char_t text_json[]="{\n\
	\"image\": {\n\
		\"width\":  800,\n\
		\"height\":  600.1,\n\
		\"hello\": \"world\",\n\
		\"int\": [1, 2, 3, 4],\n\
		\"double\": [1.1, 0.2, 0.3],\n\
		\"string\": [\"haha\", \"heihei\"]\n\
	}\n\
}";

static void _test_1(cJSON *root)
{
    hal_char_t *field = NULL;
    hal_int32_t int_val = 0;
    hal_double_t double_val = 0.0;
    hal_char_t str_val[16] = {0};

    field = "image.width";
    if (0 == UtilsJsonGetInt(root, &int_val, field, Hal_strlen(field))) {
        HalLogD("width: %d \n", int_val);
    }

    field = "image.height";
    if (0 == UtilsJsonGetDouble(root, &double_val, field, Hal_strlen(field))) {
        HalLogD("height: %f \n", double_val);
    }

    field = "image.hello";
    if (0 == UtilsJsonGetString(root, str_val, field, Hal_strlen(field))) {
        HalLogD("hello, %s \n", str_val);
    }
}

static void _test_2(cJSON *root)
{
    hal_char_t *field = NULL;
    hal_int32_t int_val = 0;
    hal_double_t double_val = 0.0;
    hal_char_t str_val[16] = {0};

    field = "image.int[2]";
    if (0 == UtilsJsonGetInt(root, &int_val, field, Hal_strlen(field))) {
        HalLogD("int val: %d \n", int_val);
    }

    field = "image.double[2]";
    if (0 == UtilsJsonGetDouble(root, &double_val, field, Hal_strlen(field))) {
        HalLogD("double val: %f \n", double_val);
    }

    field = "image.string[1]";
    if (0 == UtilsJsonGetString(root, str_val, field, Hal_strlen(field))) {
        HalLogD("string val: %s \n", str_val);
    }
}

hal_int32_t main(hal_int32_t argc, const hal_char_t *argv[])
{
    LogConfig_t log_config;
    log_config.level        = LOG_LEVEL_VERBOSE;
    log_config.color_flag   = LOG_COLOR_ON;
    log_config.buf_len      = 1024;

    HalLogInit(&log_config);

    cJSON *root = cJSON_Parse(text_json);

    _test_1(root);
    _test_2(root);

    cJSON_Delete(root);

    HalLogFinal();

    return 0;
}

