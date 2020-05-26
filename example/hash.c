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
#include <time.h>

#include "hal/hal_log.h"
#include "hal/hal_string.h"
#include "hal/hal_mem.h"

#include "utils_hash.h"

#define BUCKET_MAX_LEN  (16)
#define STR_VAL_LEN     (64)

typedef void (*handle_item_cb_t) (HashHandle_t handle, HashItem_t *hash_item);

static void _test_hash(void)
{
    return ;
    static hal_char_t *str1 = "haha";
    static hal_char_t *str2 = "haha1";
    static hal_char_t *str3 = "hehe";
    static hal_char_t *str4 = "hehe2";

    HalLogT("str1: %s,\t str1: %u \n", str1, UtilsHashGenerate(str1));
    HalLogT("str2: %s,\t str2: %u \n", str2, UtilsHashGenerate(str2));
    HalLogT("str3: %s,\t str3: %u \n", str3, UtilsHashGenerate(str3));
    HalLogT("str4: %s,\t str4: %u \n", str4, UtilsHashGenerate(str4));
}

static void _add_item(HashHandle_t handle, HashItem_t *hash_item)
{
    hal_char_t val[STR_VAL_LEN] = {0};
    snprintf(val, STR_VAL_LEN, "hello world, %ld", time(NULL));
    hash_item->val = val;

    UtilsHashItemAdd(handle, hash_item);
}

static void _get_item_val(HashHandle_t handle, HashItem_t *hash_item)
{
    hal_char_t val[STR_VAL_LEN] = {0};
    hash_item->val = val;
    UtilsHashItemGet(handle, hash_item);

    printf("val: %s \n", (hal_char_t *)hash_item->val);
}

static void _del_item(HashHandle_t handle, HashItem_t *hash_item)
{
    UtilsHashItemDel(handle, hash_item);
}

static void _handle_item_com(HashHandle_t     handle, 
                             handle_item_cb_t handle_item_cb, 
                             HashItem_t       *hash_item,
                             hal_int32_t      len)
{
    hal_char_t key[STR_VAL_LEN] = {0};

    for (hal_int32_t i = 0; i < len; i++) {
        Hal_memset(hash_item, '\0', HASH_ITEM_LEN);
        snprintf(key, STR_VAL_LEN, "key%d", i);
        hash_item->key = key;

        if (NULL != handle_item_cb) {
            handle_item_cb(handle, hash_item);
        }
    }
}

static void _test_key_val(void)
{
    HashItem_t hash_item;

    HashConfig_t config;
    config.bucket_max_len  = BUCKET_MAX_LEN;
    HashHandle_t handle = UtilsHashCreate(&config);

    hal_int32_t bucket_max_len = UtilsHashGetBucket(handle);

    _handle_item_com(handle, _add_item,     &hash_item, 32);
    UtilsHashDump(handle);
    _handle_item_com(handle, _get_item_val, &hash_item, bucket_max_len);
    _handle_item_com(handle, _del_item,     &hash_item, bucket_max_len);

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

