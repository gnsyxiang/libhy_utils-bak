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

hal_int32_t main(hal_int32_t argc, const hal_char_t *argv[])
{
    LogConfig_t log_config;
    log_config.level        = LOG_LEVEL_VERBOSE;
    log_config.color_flag   = LOG_COLOR_ON;
    log_config.buf_len      = 1024;

    HalLogInit(&log_config);

    cJSON *root = cJSON_Parse(text_json);

    hal_char_t *field = "image.int[2]";
    int val;
    if (0 == UtilsJsonGetInt(root, field, Hal_strlen(field), &val)) {
        HalLogD("int val: %d \n", val);
    }

    cJSON_Delete(root);

    HalLogFinal();

    return 0;
}

