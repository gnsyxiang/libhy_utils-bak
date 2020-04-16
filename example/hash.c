/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    hash.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    15/04 2020 22:51
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        15/04 2020      create the file
 * 
 *     last modified: 15/04 2020 22:51
 */
#include "hal/hal_log.h"

#include "utils_hash.h"

static void _test_hash(void)
{
    static hal_char_t *str1 = "haha";
    static hal_char_t *str2 = "haha1";
    static hal_char_t *str3 = "hehe";
    static hal_char_t *str4 = "hehe2";

    HalLogT("str1: %s,\t str1: %u \n", str1, UtilsHashGenerate(str1));
    HalLogT("str2: %s,\t str2: %u \n", str2, UtilsHashGenerate(str2));
    HalLogT("str3: %s,\t str3: %u \n", str3, UtilsHashGenerate(str3));
    HalLogT("str4: %s,\t str4: %u \n", str4, UtilsHashGenerate(str4));
}

static void _test_key_val(void)
{
#define BUCKET_MAX_LEN (16)
    HashConfig_t config;
    config.bucket_max_len = BUCKET_MAX_LEN;

    HashHandle_t handle = UtilsHashCreate(&config);

    HashItem_t hash_item;
    hal_char_t key[32] = {0};
    hal_char_t val[32] = {0};

    for (hal_int32_t i = 0; i < BUCKET_MAX_LEN; i++) {
        snprintf(key, 32, "key%d", i);
        snprintf(val, 32, "key%d", i);

        hash_item.key = key;
        hash_item.val = val;
        UtilsHashItemAdd(handle, &hash_item);
    }

    UtilsHashDump(handle);

    for (hal_int32_t i = 0; i < BUCKET_MAX_LEN; i++) {
        snprintf(key, 32, "key%d", i);
        snprintf(val, 32, "key%d", i);

        hash_item.key = key;
        hash_item.val = val;
        UtilsHashItemDel(handle, &hash_item);
    }

    UtilsHashDestroy(handle);
}

hal_int32_t main(hal_int32_t argc, const hal_char_t *argv[])
{
    LogConfig_t log_config;
    log_config.level        = LOG_LEVEL_VERBOSE;
    log_config.color_flag   = LOG_COLOR_ON;
    log_config.buf_len      = 1024;

    HalLogInit(&log_config);

    _test_hash();
    _test_key_val();

    HalLogFinal();

    return 0;
}
