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
    void                *val;

    struct hlist_node   list;
} item_t;
#define ITEM_LEN (sizeof(item_t))

typedef struct {
    HashConfig_t        config;

    ThreadMutexHandle_t *bucket_mutex;
    struct hlist_head   *bucket_head;
} hash_context_t;
#define HASH_CONTEXT_LEN (sizeof(hash_context_t))

typedef void (*handle_item_cb_t)(item_t *pos, HashItem_t *item);
static inline void _del_item_from_list(item_t *pos, HashItem_t *item);
static inline void _item_dump(item_t *pos, HashItem_t *item);

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

static hal_int32_t _key_to_index(hal_int32_t bucket_max_len, const hal_char_t* key)
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
    index &= (bucket_max_len - 1);

    return index;
}

static item_t *_item_init(HashConfig_t *config, HashItem_t *hash_item)
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
    Hal_strncpy(item->val, hash_item->val, len);

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

static void _traverse_item_list(hash_context_t *context, 
        handle_item_cb_t handle_item_cb, hal_uint32_t index)
{
    if (_item_dump == handle_item_cb) {
        HalLogT("bucket: %d \n", index);
    }

    item_t *pos;
    struct hlist_node *n;
    HalMutexLock(context->bucket_mutex[index]);
    hlist_for_each_entry_safe(pos, n, &context->bucket_head[index], list) {
        if (NULL != handle_item_cb) {
            handle_item_cb(pos, NULL);
        }
    }
    HalMutexUnLock(context->bucket_mutex[index]);
}

static hal_int32_t _find_item_from_list(HashHandle_t handle, 
                                        HashItem_t *hash_item,
                                        handle_item_cb_t handle_item_cb)
{
    hal_int32_t find_flag = 0;
    hash_context_t *context = handle;

    hal_uint32_t key_hash   = UtilsHashGenerate(hash_item->key);
    hal_int32_t index       = _key_to_index(context->config.bucket_max_len, hash_item->key);

    item_t *pos;
    struct hlist_node *n;
    HalMutexLock(context->bucket_mutex[index]);
    hlist_for_each_entry_safe(pos, n, &context->bucket_head[index], list) {
        if (pos->key_hash == key_hash) {
            find_flag = 1;
            if (NULL != handle_item_cb) {
                handle_item_cb(pos, hash_item);
            }
            break;
        }
    }
    HalMutexUnLock(context->bucket_mutex[index]);
    return find_flag;
}

static hash_context_t *_context_init(HashConfig_t *config)
{
    hash_context_t *context = Hal_calloc(1, HASH_CONTEXT_LEN);
    if (NULL == context) {
        HalLogE("hal calloc faild \n");
        goto L_CONTEXT_INIT_1;
    }

    context->config = *config;
    hal_int32_t len = context->config.bucket_max_len;

    context->bucket_head = Hal_calloc(1, len * sizeof(struct hlist_head));
    if (NULL == context->bucket_head) {
        HalLogE("hal calloc faild \n");
        goto L_CONTEXT_INIT_2;
    }

    context->bucket_mutex = Hal_calloc(1, len * sizeof(ThreadMutexHandle_t));
    if (NULL == context->bucket_mutex) {
        HalLogE("hal calloc faild \n");
        goto L_CONTEXT_INIT_3;
    }

    for (hal_int32_t i = 0; i < len; i++) {
        context->bucket_mutex[i] = HalMutexInit();
        INIT_HLIST_HEAD(&context->bucket_head[i]);
    }

    return context;
L_CONTEXT_INIT_3:
    Hal_free(context->bucket_head);
L_CONTEXT_INIT_2:
    Hal_free(context);
    context = NULL;
L_CONTEXT_INIT_1:
    return context;
}

static void _context_final(hash_context_t **context_tmp)
{
    hash_context_t *context = *context_tmp;
    if (NULL != context) {
        hal_int32_t len = context->config.bucket_max_len;

        for (hal_int32_t i = 0; i < len; i++) {
            _traverse_item_list(context, _del_item_from_list, i);

            if (NULL != context->bucket_mutex[i]) {
                HalMutexDestroy(context->bucket_mutex[i]);
            }
        }

        if (NULL != context->bucket_mutex) {
            Hal_free(context->bucket_mutex);
            context->bucket_mutex = NULL;
        }

        if (NULL != context->bucket_head) {
            Hal_free(context->bucket_head);
            context->bucket_head = NULL;
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

hal_int32_t UtilsHashGetBucket(HashHandle_t handle)
{
    if (NULL == handle) {
        HalLogE("the param is NULL \n");
        return -1;
    }

    hash_context_t *context = handle;

    return context->config.bucket_max_len;
}

static inline void _add_item_to_list(HashHandle_t handle, HashItem_t *hash_item)
{
    hash_context_t *context = handle;

    hal_int32_t index = _key_to_index(context->config.bucket_max_len, hash_item->key);
    item_t *item      = _item_init(&context->config, hash_item);

    HalMutexLock(context->bucket_mutex[index]);
    hlist_add_head(&item->list, &context->bucket_head[index]);
    HalMutexUnLock(context->bucket_mutex[index]);
}

static inline void _del_item_from_list(item_t *pos, HashItem_t *item)
{
    hlist_del(&pos->list);
    _item_final(&pos);
}

static inline void _replace_item_val(item_t *pos, HashItem_t *item)
{
    hal_uint32_t len = Hal_strlen(item->val);
    pos->val = Hal_calloc(1, ALIGN4_UP(len));
    if (NULL == pos->val) {
        HalLogE("calloc failed \n");
        return ;
    }

    if (NULL != pos->val) {
        Hal_free(pos->val);
        pos->val = NULL;
    }

    Hal_strncpy(pos->val, item->val, len);
}

hal_int32_t UtilsHashItemAdd(HashHandle_t handle, HashItem_t *hash_item)
{
    if (NULL == handle || NULL == hash_item) {
        HalLogE("the param is NULL \n");
        return -1;
    }

    hal_int32_t find_flag = 0;
    find_flag = _find_item_from_list(handle, hash_item, _replace_item_val);

    if (0 == find_flag) {
        _add_item_to_list(handle, hash_item);
    }

    return HAL_NO_ERR;
}

hal_int32_t UtilsHashItemDel(HashHandle_t handle, HashItem_t *hash_item)
{
    if (NULL == handle) {
        HalLogE("the param is NULL \n");
        return -1;
    }

    return _find_item_from_list(handle, hash_item, _del_item_from_list);
}

static inline void _get_item_val(item_t *pos, HashItem_t *item)
{
    Hal_strncpy(item->val, pos->val, Hal_strlen((hal_char_t *)pos->val));
}

static inline void _item_dump(item_t *pos, HashItem_t *item)
{
    HalLogT("key_hash: %u, \tval: %s\n", pos->key_hash, (hal_char_t *)pos->val);
}

hal_int32_t UtilsHashItemGet(HashHandle_t handle, HashItem_t *hash_item)
{
    if (NULL == handle) {
        HalLogE("the param is NULL \n");
        return -1;
    }

    return _find_item_from_list(handle, hash_item, _get_item_val);
}

void UtilsHashDump(HashHandle_t handle)
{
    if (NULL == handle) {
        HalLogE("the param is NULL \n");
        return ;
    }

    hash_context_t *context = handle;

    for (hal_int32_t i = 0; i < context->config.bucket_max_len; i++) {
        _traverse_item_list(context, _item_dump, i);
    }
}

