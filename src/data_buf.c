/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    databuf.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/11 2019 14:27
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/11 2019      create the file
 * 
 *     last modified: 05/11 2019 14:27
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "data_buf.h"
#include "utils.h"

#define databuf_log(fmt, ...) \
    printf("<%s:%d, result: %s> " fmt, \
           __func__, __LINE__, strerror(errno), ##__VA_ARGS__);

typedef struct {
    int m_head;
    int m_tail;

    DataBufConfig_t m_config;
    int m_remain_size;

    char buf[0];
} databuf_t;

static void _databuf_dump_print(int start, int end, char *buf)
{
    for (int i = start; i < end; i++) {
        printf("%02x ", buf[i]);
    }
}

static inline void _databuf_dump(databuf_t *databuf)
{
    if (databuf->m_head < databuf->m_tail || \
        (databuf->m_head == databuf->m_tail && databuf->m_remain_size == 0)) {
        _databuf_dump_print(databuf->m_tail, \
                            databuf->m_config.m_size, \
                            databuf->buf + databuf->m_tail);
        _databuf_dump_print(0, databuf->m_head, databuf->buf);
    } else {
        _databuf_dump_print(databuf->m_tail, \
                            databuf->m_head, \
                            databuf->buf + databuf->m_tail);
    }
    printf("\n");
}

void DataBufDump(void *handle)
{
    if (!handle) {
        databuf_log("the handle is NULL\n");
        return ;
    }
    _databuf_dump((databuf_t *)handle);
}

static inline databuf_t *_databuf_init(DataBufConfig_t *config)
{
    int size = ALIGN4(config->m_size);
    databuf_t *databuf = calloc(1, DATA_TYPE_LEN(databuf_t) + size);
    if (!databuf) {
        databuf_log("calloc faild \n");
        return NULL;
    }

    databuf->m_remain_size      = size;
    databuf->m_config.m_size    = size;
    databuf->m_config.m_ability = config->m_ability;

    databuf->m_tail = 0;
    databuf->m_head = 0;

    return databuf;
}

static inline void _databuf_final(databuf_t *databuf)
{
    free(databuf);
}

void *DataBufInit(DataBufConfig_t *config)
{
    if (!config) {
        databuf_log("the databuf_config is NULL \n");
        return NULL;
    }
    if (config->m_size <= 0) {
        databuf_log("the size is less then zero \n");
        return NULL;
    }
    return _databuf_init(config);
}

void DataBufFinal(void *handle)
{
    if (!handle) {
        databuf_log("the handle is NULL\n");
        return ;
    }
    _databuf_final((databuf_t *)handle);
}

static inline int _judge_full_empty(databuf_t *databuf, int flag)
{
    if (flag == 1) {
        return (databuf->m_remain_size == databuf->m_config.m_size) ? 1 : 0;
    } else {
        return (databuf->m_remain_size == 0) ? 1 : 0;
    }
}

int DataBufIsFull(void *handle)
{
    if (!handle) {
        databuf_log("the handle is NULL\n");
        return -1;
    }
    return _judge_full_empty((databuf_t *)handle, 1);
}

int DataBufIsEmpty(void *handle)
{
    if (!handle) {
        databuf_log("the handle is NULL\n");
        return -1;
    }
    return _judge_full_empty((databuf_t *)handle, 0);
}

static int _databuf_write(databuf_t *databuf, void *buf, int len)
{
    int m_tail = databuf->m_tail;

    if (m_tail + len <= databuf->m_config.m_size) {
        memcpy(databuf->buf + m_tail, buf, len);
    } else {
        int sz = databuf->m_config.m_size - m_tail;
        memcpy(databuf->buf + m_tail, buf, sz);
        memcpy(databuf->buf, buf + sz, len - sz);
    }

    printf("--1----m_tail: %d \n", databuf->m_tail);
    databuf->m_tail += len;
    printf("--2----m_tail: %d \n", databuf->m_tail);
    databuf->m_tail %= databuf->m_config.m_size;
    printf("--3----m_tail: %d \n", databuf->m_tail);

    databuf->m_remain_size -= len;
    printf("--4----m_remain_size: %d \n", databuf->m_remain_size);

    return len;
}

int DataBufWrite(void *handle, void *buf, int len)
{
    if (!handle) {
        databuf_log("the handle is NULL\n");
        return -1;
    }
    if (!buf || len <=0) {
        databuf_log("the buf is NULL or the len less then zero \n");
        return -1;
    }
    return _databuf_write((databuf_t *)handle, buf, len);
}

static int _databuf_read(databuf_t *databuf, void *buf, int len)
{
    int m_head = databuf->m_head;
    
    if (m_head + len < databuf->m_config.m_size) {
        memcpy(buf, databuf->buf, len);
    } else {
        int sz = databuf->m_config.m_size - m_head;
        memcpy(buf, databuf->buf + m_head, sz);
        memcpy(buf + sz, databuf->buf, len - sz);
    }

    databuf->m_head += len;
    databuf->m_head %= databuf->m_config.m_size;

    databuf->m_remain_size += len;

    return len;
}

int DataBufRead(void *handle, void *buf, int len)
{
    if (!handle) {
        databuf_log("the handle is NULL\n");
        return -1;
    }
    if (!buf || len <=0) {
        databuf_log("the buf is NULL or the len less then zero \n");
        return -1;
    }
    return _databuf_read((databuf_t *)handle, buf, len);
}

#if 0
int DataBufPeekRead(void *handle, void *buf, int len)
{
    return 0;
}
#endif

