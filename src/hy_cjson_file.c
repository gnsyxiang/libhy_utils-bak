/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    hy_cjson_file.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/04 2020 18:16
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/04 2020      create the file
 * 
 *     last modified: 06/04 2020 18:16
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hy_cjson_file.h"
#include "hy_cjson.h"
#include "hy_log.h"

#ifdef USE_DEBUG
    #define ALONE_DEBUG 1
    #define LOG_CATEGORY_TAG "hy_cjson_file"
#endif

static char *_get_file_content(char *file)
{
    if (0 != access(file, 0)) {
        LOGE("the file not exist \n");
        return NULL;
    }

    int32_t fd = open(file, O_RDONLY, 0644);
    if (fd < 0) {
        LOGE("open file failed \n");
        return NULL;
    }

    off_t offset = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char *file_content = calloc(1, offset);
    if (NULL == file_content) {
        LOGE("calloc failed \n");
        return NULL;
    }

    if (offset != read(fd, file_content, offset)) {
        LOGE("read len failed \n");
        return NULL;
    }

    close(fd);

    return file_content;
}

static void _put_file_content(char **file_content)
{
    if (NULL != *file_content) {
        free(*file_content);
        *file_content = NULL;
    }
}

cJSON *HyJsonFileGetRoot(char *file)
{
    if (NULL == file) {
        LOGE("the paran is NULL \n");
        return NULL;
    }

    char *file_content = _get_file_content(file);
    if (NULL == file_content) {
        LOGE("get file content failed \n");
        return NULL;
    }

    cJSON *root = cJSON_Parse(file_content);
    if (NULL == root) {
        LOGE("cjson parse failed \n");
        return NULL;
    }

    _put_file_content(&file_content);
    return root;
}

void HyJsonFilePutRoot(cJSON **root)
{
    if (NULL == root) {
        LOGE("the param is NULL \n");
        return ;
    }

    cJSON_Delete(*root);
    *root = NULL;
}

typedef int32_t (*file_get_cb_t)(cJSON *root, void *val, char *field,
                                     uint32_t field_len);

static int32_t _file_get_com(char *file, void *val, 
                                 char *field, uint32_t field_len,
                                 file_get_cb_t file_get_cb)
{
    if (NULL == file || NULL == val) {
        LOGE("the paran is NULL \n");
        return -1;
    }

    cJSON *root = HyJsonFileGetRoot(file);
    if (NULL == root) {
        LOGE("json file get root failed \n");
        return -1;
    }

    if (NULL != file_get_cb) {
        if (0 != file_get_cb(root, val, field, field_len)) {
            LOGE("parse int json failed \n");
            return -1;
        }
    }

    HyJsonFilePutRoot(&root);

    return 0;
}

int32_t HyJsonFileGetInt(char *file, int32_t *val, char *field, uint32_t field_len)
{
    return _file_get_com(file, val, field, field_len, (file_get_cb_t)HyJsonGetInt);
}

int32_t HyJsonFileGetDouble(char *file, double *val, char *field, uint32_t field_len)
{
    return _file_get_com(file, val, field, field_len, (file_get_cb_t)HyJsonGetDouble);
}

int32_t HyJsonFileGetString(char *file, char *val, char *field, uint32_t field_len)
{
    return _file_get_com(file, val, field, field_len, (file_get_cb_t)HyJsonGetString);
}

