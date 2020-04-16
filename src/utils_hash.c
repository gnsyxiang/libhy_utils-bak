/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    utils_hash.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    15/04 2020 22:43
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        15/04 2020      create the file
 * 
 *     last modified: 15/04 2020 22:43
 */
#include "hal/hal_log.h"
#include "hal/hal_mem.h"
#include "hal/hal_string.h"
#include "hal/hal_mutex.h"

#include "utils_hash.h"
#include "utils_list.h"

typedef struct item {
    hal_uint32_t        key_hash;
    hal_char_t          *val;

    struct list_head    list;
} item_t;
#define ITEM_LEN (sizeof(item_t))

typedef struct {
    HashConfig_t        config;

    struct list_head    *bucket;
} hash_context_t;
#define HASH_CONTEXT_LEN (sizeof(hash_context_t))

static inline item_t *_item_init(HashItem_t *hash_item)
{
    item_t *item = Hal_calloc(1, ITEM_LEN);
    if (NULL == item) {
        HalLogE("hal calloc faild \n");
        goto L_ITEM_INIT_1;
    }

    hal_uint32_t len = Hal_strlen(hash_item->val);
    item->val = Hal_calloc(1, ALIGN4_UP(len));
    if (NULL == item) {
        HalLogE("hal calloc faild \n");
        goto L_ITEM_INIT_2;
    }

    item->key_hash = UtilsHash(hash_item->key);
    Hal_strncpy(item->val, hash_item->val, len);

    return item;
L_ITEM_INIT_2:
    Hal_free(item);
    item = NULL;
L_ITEM_INIT_1:
    return item;
}

static inline void _item_final(item_t **item_tmp)
{
    item_t *item = *item_tmp;
    if (NULL != item) {
        if (NULL != item->val) {
            Hal_free(item->val);
            item->val = NULL;
        }

        Hal_free(item);
        *item_tmp = NULL;
    }
}

static inline void _item_dump(item_t *item)
{
    HalLogT("key_hash: %u \n", item->key_hash);
    HalLogT("val: %s \n", item->val);
}

typedef void (*dump_item_cb_t)(item_t *item);
static inline void _del_all_item_in_list(hash_context_t *context, dump_item_cb_t dump_item_cb)
{
    item_t *pos, *n;
    for (hal_uint32_t i = 0; i < context->config.bucket_max_len; i++) {
        list_for_each_entry_safe(pos, n, &context->bucket[i], list) {
            list_del(&pos->list);
            if (NULL != dump_item_cb) {
                dump_item_cb(pos);
            }
            _item_final(&pos);
        }
    }
}

static inline hash_context_t *_context_init(HashConfig_t *config)
{
    hash_context_t *context = Hal_calloc(1, HASH_CONTEXT_LEN);
    if (NULL == context) {
        HalLogE("hal calloc faild \n");
        goto L_CONTEXT_INIT_1;
    }

    context->config = *config;

    hal_int32_t len = context->config.bucket_max_len * sizeof(struct list_head);
    context->bucket = Hal_calloc(1, len);
    if (NULL == context->bucket) {
        HalLogE("hal calloc faild \n");
        goto L_CONTEXT_INIT_2;
    }

    for (hal_uint32_t i = 0; i < context->config.bucket_max_len; i++) {
        INIT_LIST_HEAD(&context->bucket[i]);
    }

    return context;
L_CONTEXT_INIT_2:
    Hal_free(context);
    context = NULL;
L_CONTEXT_INIT_1:
    return context;
}


static inline void _context_final(hash_context_t **context_tmp)
{
    hash_context_t *context = *context_tmp;
    if (NULL != context) {
        _del_all_item_in_list(context, NULL);

        if (NULL != context->bucket) {
            Hal_free(context->bucket);
        }

        Hal_free(context);
        *context_tmp = NULL;
    }
}

HashHandle_t UtilsHashCreate(HashConfig_t *config)
{
    hash_context_t *context = _context_init(config);
    if (NULL == context) {
        HalLogE("hash context init failed \n");
        return NULL;
    }

    return context;
}

void UtilsHashDestroy(HashHandle_t handle)
{
    if (NULL == handle) {
        HalLogE("the param is NULL \n");
        return ;
    }

    hash_context_t *context = handle;
    _context_final(&context);
}

static hal_int32_t _key_to_index(HashConfig_t *config, const hal_char_t* key)
{
    if (NULL == key) {
        HalLogE("the param is NULL \n");
        return -1;
    }

    hal_int32_t len   = Hal_strlen(key);
    hal_int32_t index = (hal_int32_t)key[0];

    for (hal_int32_t i = 1; i < len; ++i) {
        index *= 1103515245 + (hal_int32_t)key[i];
    }

    index >>= 27;
    index &= (config->bucket_max_len - 1);

    return index;
}

static void _item_replace_val(item_t *item, HashItem_t *hash_item)
{
    hal_uint32_t len = Hal_strlen(hash_item->val);

    if (Hal_strlen(item->val) != len) {
        Hal_free(item->val);

        hal_char_t *val = Hal_calloc(1, ALIGN4_UP(len));
        if (NULL == val) {
            HalLogE("hal calloc faild \n");
        }

        Hal_strncpy(val, hash_item->val, len);
        item->val = val;
    }
}

hal_int32_t UtilsHashAdd(HashHandle_t handle, HashItem_t *hash_item)
{
    if (NULL == handle || NULL == hash_item) {
        HalLogE("the param is NULL \n");
        return -1;
    }

    hash_context_t *context = handle;
    hal_uint32_t key_hash   = UtilsHash(hash_item->key);
    hal_int32_t index       = _key_to_index(&context->config, hash_item->key);

    hal_int32_t find_flag = 0;
    item_t *pos;
    list_for_each_entry(pos, &context->bucket[index], list) {
        if (pos->key_hash == key_hash) {
            find_flag = 1;
            _item_replace_val(pos, hash_item);
            break;
        }
    }

    if (0 == find_flag) {
        item_t *item = _item_init(hash_item);
        list_add_tail(&item->list, &context->bucket[index]);
    }

    return HAL_NO_ERR;
}

hal_int32_t UtilsHashDel(HashHandle_t handle, HashItem_t *hash_item)
{
    if (NULL == handle) {
        HalLogE("the param is NULL \n");
        return -1;
    }

    hash_context_t *context = handle;
    hal_uint32_t key_hash   = UtilsHash(hash_item->key);
    hal_int32_t index       = _key_to_index(&context->config, hash_item->key);

    hal_int32_t ret = -1;
    item_t *pos, *n;
    list_for_each_entry_safe(pos, n, &context->bucket[index], list) {
        if (pos->key_hash == key_hash) {
            ret = 0;
            list_del(&pos->list);
            _item_final(&pos);
            break;
        }
    }

    return ret;
}

void UtilsHashDump(HashHandle_t handle)
{
    if (NULL == handle) {
        HalLogE("the param is NULL \n");
        return ;
    }

    hash_context_t *context = handle;

    _del_all_item_in_list(context, _item_dump);
}

hal_uint32_t UtilsHash(const hal_char_t *key)
{
    if (NULL == key) {
        HalLogE("hash key input is NULL \n");
        return 0;
    }

    hal_uint32_t seed = 31; // 31 131 1313 13131 131313 etc..
    hal_uint32_t hash = 0;

    while (*key) {
        hash = hash * seed + (*key++);
    }

    return hash;
}

