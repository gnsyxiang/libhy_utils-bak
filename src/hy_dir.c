/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_dir.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    16/10 2021 11:44
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        16/10 2021      create the file
 * 
 *     last modified: 16/10 2021 11:44
 */
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#include "hy_dir.h"

#include "hy_type.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

static void _handle_sub_dir(const char *path, char *name,
        HyDirReadCb_t handle_cb, void *args)
{
    size_t len = strlen(path) + strlen(name) + 1 + 1; // 1 for space('\0'), 1 for '/'
    char *sub_path = malloc(len);
    if (!sub_path) {
        LOGE("malloc failed \n");
        return ;
    }
    memset(sub_path, '\0', len);

    snprintf(sub_path, len, "%s/%s", path, name);
    HyDirRead(sub_path, handle_cb, args);

    free(sub_path);
}

int32_t HyDirRead(const char *path, HyDirReadCb_t handle_cb, void *args)
{
    DIR *dir;
    struct dirent *ptr;

    dir = opendir(path);
    if (!dir) {
        LOGE("opendir failed \n");
        return -1;
    }

    while ((ptr = readdir(dir)) != NULL) {
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) {
            continue;
        }

        switch (ptr->d_type) {
            case DT_REG:
                if (handle_cb) {
                    handle_cb(path, ptr->d_name, DT_REG, args);
                }
                break;

            case DT_DIR:
                _handle_sub_dir(path, ptr->d_name, handle_cb, args);
                break;

            default:
                break;
        }
    }

    closedir(dir);

    return 0;
}
