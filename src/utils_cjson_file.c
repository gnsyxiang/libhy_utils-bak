/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    utils_cjson_file.c
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
#include "utils_cjson_file.h"
#include "utils_cjson.h"

#include "hal/hal_file.h"
#include "hal/hal_log.h"
#include "hal/hal_mem.h"

static hal_char_t *_get_file_content(hal_char_t *file)
{
    if (0 != Hal_access(file, 0)) {
        HalLogE("the file not exist \n");
        return NULL;
    }

    hal_int32_t fd = Hal_open(file, O_RDONLY, 0644);
    if (fd < 0) {
        HalLogE("open file failed \n");
        return NULL;
    }

    off_t offset = Hal_lseek(fd, 0, SEEK_END);
    Hal_lseek(fd, 0, SEEK_SET);

    hal_char_t *file_content = Hal_calloc(1, offset);
    if (NULL == file_content) {
        HalLogE("calloc failed \n");
        return NULL;
    }

    if (offset != Hal_read(fd, file_content, offset)) {
        HalLogE("read len failed \n");
        return NULL;
    }

    Hal_close(fd);

    return file_content;
}

static void _put_file_content(hal_char_t **file_content)
{
    if (NULL != *file_content) {
        Hal_free(*file_content);
        *file_content = NULL;
    }
}

cJSON *UtilsJsonFileGetRoot(hal_char_t *file)
{
    if (NULL == file) {
        HalLogE("the paran is NULL \n");
        return NULL;
    }

    hal_char_t *file_content = _get_file_content(file);
    if (NULL == file_content) {
        HalLogE("get file content failed \n");
        return NULL;
    }

    cJSON *root = cJSON_Parse(file_content);
    if (NULL == root) {
        HalLogE("cjson parse failed \n");
        return NULL;
    }

    _put_file_content(&file_content);
    return root;
}

void UtilsJsonFilePutRoot(cJSON **root)
{
    if (NULL == root) {
        HalLogE("the param is NULL \n");
        return ;
    }

    cJSON_Delete(*root);
    *root = NULL;
}

hal_int32_t UtilsJsonFileGetInt(hal_char_t *file, hal_int32_t *val, 
        hal_char_t *field, hal_uint32_t field_len)
{
    if (NULL == file || NULL == val) {
        HalLogE("the paran is NULL \n");
        return -1;
    }

    cJSON *root = UtilsJsonFileGetRoot(file);
    if (NULL == root) {
        HalLogE("json file get root failed \n");
        return -1;
    }

    if (0 != UtilsJsonGetInt(root, val, field, field_len)) {
        HalLogE("parse int json failed \n");
        return -1;
    }

    UtilsJsonFilePutRoot(&root);

    return 0;
}

