/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    cjson.c
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
#include <stdio.h>
#include <string.h>

#include "hy_cjson.h"
#include "hy_cjson_file.h"

#include "hy_fifo.h"
#include "hy_log.h"

#ifdef USE_DEBUG
    #define ALONE_DEBUG 1
    #define LOG_CATEGORY_TAG "hy_cjson_test"
#endif

static void _test_1(cJSON *root)
{
    char *field = NULL;
    int32_t int_val = 0;
    double double_val = 0.0;
    char str_val[16] = {0};

    field = "image.width";
    if (0 == HyJsonGetInt(root, &int_val, field, strlen(field))) {
        LOGD("width: %d \n", int_val);
    }

    field = "image.height";
    if (0 == HyJsonGetDouble(root, &double_val, field, strlen(field))) {
        LOGD("height: %f \n", double_val);
    }

    field = "image.hello";
    if (0 == HyJsonGetString(root, str_val, field, strlen(field))) {
        LOGD("hello, %s \n", str_val);
    }

    printf("\n\n");
}

static void _test_2(cJSON *root)
{
    char *field = NULL;
    int32_t int_val = 0;
    double double_val = 0.0;
    char str_val[16] = {0};

    field = "image.int[2]";
    if (0 == HyJsonGetInt(root, &int_val, field, strlen(field))) {
        LOGD("int val: %d \n", int_val);
    }

    field = "image.double[2]";
    if (0 == HyJsonGetDouble(root, &double_val, field, strlen(field))) {
        LOGD("double val: %f \n", double_val);
    }

    field = "image.string[1]";
    if (0 == HyJsonGetString(root, str_val, field, strlen(field))) {
        LOGD("string val: %s \n", str_val);
    }

    printf("\n\n");
}

static void _test_json_str(void)
{
    char text_json[]="{\n\
    	\"image\": {\n\
    		\"width\":  800,\n\
    		\"height\":  600.1,\n\
    		\"hello\": \"world\",\n\
    		\"int\": [1, 2, 3, 4],\n\
    		\"double\": [1.1, 0.2, 0.3],\n\
    		\"string\": [\"haha\", \"heihei\"]\n\
    	}\n\
    }";

    cJSON *root = cJSON_Parse(text_json);

    _test_1(root);
    _test_2(root);

    cJSON_Delete(root);
}

#define JSON_CONFIG_FILE "../res/cjson.config"

static void _test_3(void)
{
    char *field = NULL;
    int32_t int_val = 0;
    double double_val = 0.0;
    char str_val[16] = {0};

    field = "image.int[2]";
    if (0 == HyJsonFileGetInt(JSON_CONFIG_FILE, &int_val, field, strlen(field))) {
        LOGD("int val: %d \n", int_val);
    }

    field = "image.double[2]";
    if (0 == HyJsonFileGetDouble(JSON_CONFIG_FILE, &double_val, field, strlen(field))) {
        LOGD("double val: %f \n", double_val);
    }

    field = "image.string[1]";
    if (0 == HyJsonFileGetString(JSON_CONFIG_FILE, str_val, field, strlen(field))) {
        LOGD("string val: %s \n", str_val);
    }

    printf("\n\n");
}

static void _test_json_file(void)
{
    cJSON *root = HyJsonFileGetRoot(JSON_CONFIG_FILE);
    if (NULL == root) {
        LOGE("get root failed \n");
        return;
    }

    _test_1(root);
    _test_2(root);

    HyJsonFilePutRoot(&root);

    _test_3();
}

int32_t main(int32_t argc, const char *argv[])
{
    HyLogCreate(USE_LOG_LEVEL, 1024);

    _test_json_str();
    _test_json_file();

    HyLogDestory();

    return 0;
}

