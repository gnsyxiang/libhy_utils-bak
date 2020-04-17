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

#define HASH_OFFSET_LEN (sizeof(hal_uint32_t))

typedef struct item {
    hal_uint32_t        key_hash;
    hal_char_t *val;

    struct list_head    list;
} item_t;
#define ITEM_LEN (sizeof(item_t))

typedef struct {
    HashConfig_t        config;

    struct list_head    *bucket;
} hash_context_t;
#define HASH_CONTEXT_LEN (sizeof(hash_context_t))

static inline item_t *_item_init(HashConfig_t *config, HashItem_t *hash_item)
{
    item_t *item = Hal_calloc(1, ITEM_LEN);
    if (NULL == item) {
        HalLogE("hal calloc faild \n");
        goto L_ITEM_INIT_1;
    }

    if (HASH_VAL_FLAG == config->val_offset_flag) {
        hal_uint32_t len = Hal_strlen(hash_item->val);
        item->val = Hal_calloc(1, ALIGN4_UP(len));
        if (NULL == item) {
            HalLogE("hal calloc faild \n");
            goto L_ITEM_INIT_2;
        }
        Hal_strncpy(item->val, hash_item->val, len);
    } else {
        hal_uint32_t len = HASH_OFFSET_LEN;
        item->val = Hal_calloc(1, len);
        if (NULL == item) {
            HalLogE("hal calloc faild \n");
            goto L_ITEM_INIT_2;
        }
        Hal_memcpy(item->val, &hash_item->offset, len);
    }

    item->key_hash = UtilsHashGenerate(hash_item->key);

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

static inline void _item_dump(HashConfig_t *config, item_t *item, void *args)
{
    if (HASH_VAL_FLAG == config->val_offset_flag) {
        HalLogT("key_hash: %u, \tval: %s \n", item->key_hash, (hal_char_t *)item->val);
    } else {
        HalLogT("key_hash: %u, \tval: %d \n", item->key_hash, *(hal_uint32_t *)item->val);
    }
}

typedef void (*handle_item_cb_t)(HashConfig_t *config, item_t *item, void *args);
static inline void _del_all_item_in_list(hash_context_t *context, handle_item_cb_t handle_item_cb)
{
    item_t *pos, *n;
    for (hal_uint32_t i = 0; i < context->config.bucket_max_len; i++) {
        list_for_each_entry_safe(pos, n, &context->bucket[i], list) {
            list_del(&pos->list);
            if (NULL != handle_item_cb) {
                handle_item_cb(&context->config, pos, NULL);
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

void UtilsHashDump(HashHandle_t handle)
{
    if (NULL == handle) {
        HalLogE("the param is NULL \n");
        return ;
    }

    hash_context_t *context = handle;

    _del_all_item_in_list(context, _item_dump);
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

typedef enum {
    HANDLE_ITEM_GET,
    HANDLE_ITEM_SET,
} handle_item_type;

static hal_int32_t _hash_find_item(HashHandle_t handle, HashItem_t *hash_item,
        handle_item_cb_t handle_item_cb, hal_int32_t type)
{
    hash_context_t *context = handle;
    hal_uint32_t key_hash   = UtilsHashGenerate(hash_item->key);
    hal_int32_t index       = _key_to_index(&context->config, hash_item->key);

    hal_int32_t find_flag = 0;
    item_t *pos;
    list_for_each_entry(pos, &context->bucket[index], list) {
        if (pos->key_hash == key_hash) {
            find_flag = 1;
            if (NULL != handle_item_cb) {
                if (type == HANDLE_ITEM_GET) {
                    handle_item_cb(&context->config, pos, pos->val);
                } else {
                    if (HASH_VAL_FLAG == context->config.val_offset_flag) {
                        handle_item_cb(&context->config, pos, hash_item->val);
                    } else {
                        handle_item_cb(&context->config, pos, &hash_item->offset);
                    }
                }
            }
            break;
        }
    }
    return find_flag;
}

static void _replace_item_val(HashConfig_t *config, item_t *item, void *val)
{
    if (HASH_VAL_FLAG == config->val_offset_flag) {
        hal_uint32_t len = Hal_strlen(val);

        if (Hal_strlen(val) != len) {
            Hal_free(val);

            hal_char_t *val_new = Hal_calloc(1, ALIGN4_UP(len));
            if (NULL == val_new) {
                HalLogE("hal calloc faild \n");
            }

            Hal_strncpy(val_new, val, len);
            item->val = val;
        }
    } else {
        hal_uint32_t val_old = *(hal_uint32_t *)(item->val);
        hal_uint32_t val_new = *(hal_uint32_t *)val;
        if (val_old != val_new) {
            val_old = val_new;
        }
    }
}

hal_int32_t UtilsHashItemAdd(HashHandle_t handle, HashItem_t *hash_item)
{
    if (NULL == handle || NULL == hash_item) {
        HalLogE("the param is NULL \n");
        return -1;
    }

    hal_int32_t find_flag = 0;
    find_flag = _hash_find_item(handle, hash_item, _replace_item_val, HANDLE_ITEM_SET);

    if (0 == find_flag) {
        hash_context_t *context = handle;
        hal_int32_t index = _key_to_index(&context->config, hash_item->key);
        item_t *item = _item_init(&context->config, hash_item);
        list_add_tail(&item->list, &context->bucket[index]);
    }

    return HAL_NO_ERR;
}

static void _del_item(HashConfig_t *config, item_t *item, void *val)
{
    list_del(&item->list);
    _item_final(&item);
}

hal_int32_t UtilsHashItemDel(HashHandle_t handle, HashItem_t *hash_item)
{
    if (NULL == handle) {
        HalLogE("the param is NULL \n");
        return -1;
    }

    return _hash_find_item(handle, hash_item, _del_item, HANDLE_ITEM_SET);
}

static void _get_item_val(HashConfig_t *config, item_t *item, void *val)
{
    printf("--1\n");
    if (HASH_VAL_FLAG == config->val_offset_flag) {
    printf("--2\n");
        Hal_strncpy(val, item->val, Hal_strlen(item->val));
    } else {
    printf("--3\n");
        printf("-------offset: %d \n", *(hal_uint32_t *)item->val);
        Hal_memcpy(val, item->val, HASH_OFFSET_LEN);
    }
    printf("--4\n");
}

hal_int32_t UtilsHashItemGet(HashHandle_t handle, HashItem_t *hash_item)
{
    if (NULL == handle) {
        HalLogE("the param is NULL \n");
        return -1;
    }

    printf("--0\n");
    return _hash_find_item(handle, hash_item, _get_item_val, HANDLE_ITEM_GET);
}

hal_uint32_t UtilsHashGenerate(const hal_char_t *key)
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

