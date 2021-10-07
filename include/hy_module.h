/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_module.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    07/07 2021 17:11
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        07/07 2021      create the file
 * 
 *     last modified: 07/07 2021 17:11
 */
#ifndef __LIBHY_UTILS_INCLUDE_HY_MODULE_H_
#define __LIBHY_UTILS_INCLUDE_HY_MODULE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void *(*create_t)(void *config);
typedef void (*destroy_t)(void **handle);

typedef struct {
    const char  *name;
    void **handle;
    void *config;
    create_t create;
    destroy_t destroy;
} module_create_t;

typedef struct {
    const char *name;
    void **handle;
    destroy_t destroy;
} module_destroy_t;

#define RUN_CREATE(module)                                          \
    do {                                                            \
        int32_t i;                                                  \
        int32_t len = HyUtilsArrayCnt(module);                      \
        for (i = 0; i < len; ++i) {                                 \
            module_create_t *create = &module[i];                   \
            if (create->create) {                                   \
                *create->handle = create->create(create->config);   \
                if (!*create->handle) {                             \
                    LOGE("%s create error \n", create->name);       \
                    break;                                          \
                }                                                   \
            }                                                       \
        }                                                           \
                                                                    \
        if (i < len) {                                              \
            int32_t j;                                              \
            for (j = i - 1; j >= 0; j--) {                          \
                module_create_t *create = &module[j];               \
                if (create->destroy) {                              \
                    create->destroy(create->handle);                \
                }                                                   \
            }                                                       \
            return NULL;                                            \
        }                                                           \
    } while(0)

#define RUN_DESTROY(module)                                         \
    do {                                                            \
        int32_t i;                                                  \
        for (i = 0; i < HyUtilsArrayCnt(module); ++i) {             \
            module_destroy_t *destroy = &module[i];                 \
            if (destroy->destroy) {                                 \
                destroy->destroy(destroy->handle);                  \
            }                                                       \
        }                                                           \
    } while(0)

#ifdef __cplusplus
}
#endif

#endif

