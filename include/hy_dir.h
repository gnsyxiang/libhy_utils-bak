/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_dir.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    16/10 2021 11:43
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        16/10 2021      create the file
 * 
 *     last modified: 16/10 2021 11:43
 */
#ifndef __LIBHY_UTILS_INCLUDE_HY_DIR_H_
#define __LIBHY_UTILS_INCLUDE_HY_DIR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef void (*HyDirReadCb_t)(const char *path, const char *name,
        uint8_t type, void *args);

int32_t HyDirRead(const char *path, HyDirReadCb_t read_cb, void *args);

#ifdef __cplusplus
}
#endif

#endif

