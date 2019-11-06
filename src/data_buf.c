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
#include <pthread.h>
#include<limits.h>

#include "data_buf.h"
#include "utils.h"
#include "time_wrapper.h"

// #define DATABUF_DEBUG
#define databuf_log(fmt, ...) \
    printf("<%s:%d, result: %s> " fmt, \
           __func__, __LINE__, strerror(errno), ##__VA_ARGS__);

enum {
    READ_DATA,
    PEEK_READ_DATA,
};

typedef struct {
    pthread_mutex_t m_mutex;
    pthread_cond_t  m_read_cond;
    pthread_cond_t  m_write_cond;

    int m_read_cnt;
    int m_write_cnt;
} ThreadSafety_t;

typedef struct {
    int m_head;
    int m_tail;
    int m_remain_size;
    int m_is_init;

    DataBufConfig_t m_config;
    ThreadSafety_t m_thread;

    char buf[0];
} databuf_t;

#define _databuf_dump_print_hex(start, end, buf)    \
    do {                                            \
        for (int i = start; i < end; i++) {         \
            printf("%c ", buf[i]);                \
        }                                           \
    } while(0)

static void _databuf_dump_print(int start, int end, char *buf, int size)
{
    if (end - start > 0) {
        _databuf_dump_print_hex(start, end, buf);
    } else {
        _databuf_dump_print_hex(start, size, buf);
        _databuf_dump_print_hex(0, end, buf);
    }
    printf("\n");
}

static void _databuf_dump(databuf_t *databuf)
{
    if (databuf->m_config.m_ability == DATABUF_ABILITY_THREAD_SAFETY) {
        pthread_mutex_lock(&databuf->m_thread.m_mutex);
    }
    if (databuf->m_head == databuf->m_tail && databuf->m_remain_size != 0) {
        databuf_log("the databuf is empty \n");
        return;
    }
    _databuf_dump_print(databuf->m_head, databuf->m_tail, databuf->buf, databuf->m_config.m_size);
    if (databuf->m_config.m_ability == DATABUF_ABILITY_THREAD_SAFETY) {
        pthread_mutex_unlock(&databuf->m_thread.m_mutex);
    }
}

void DataBufDump(void *handle)
{
    if (!handle) {
        databuf_log("the handle is NULL\n");
        return ;
    }
    _databuf_dump((databuf_t *)handle);
}

static inline void _databuf_config_init(databuf_t *databuf, int size, DataBufAbility_t ability)
{
    databuf->m_config.m_size    = size;
    databuf->m_config.m_ability = ability;
}

static inline void _thread_safety_init(databuf_t *databuf)
{
    pthread_mutex_init(&databuf->m_thread.m_mutex, NULL);
    pthread_cond_init (&databuf->m_thread.m_read_cond, NULL);
    pthread_cond_init (&databuf->m_thread.m_write_cond, NULL);
}

static inline void _thread_safety_final(databuf_t *databuf)
{
    pthread_mutex_destroy(&databuf->m_thread.m_mutex);
    pthread_cond_destroy (&databuf->m_thread.m_read_cond);
    pthread_cond_destroy (&databuf->m_thread.m_write_cond);
}

static inline databuf_t *_databuf_init(DataBufConfig_t *config)
{
    int size = ALIGN4(config->m_size);
    databuf_log("the size is: %d \n", size);
    databuf_t *databuf = calloc(1, DATA_TYPE_LEN(databuf_t) + size);
    if (!databuf) {
        databuf_log("calloc faild \n");
        return NULL;
    }

    databuf->m_remain_size = size;
    databuf->m_tail        = 0;
    databuf->m_head        = 0;

    _databuf_config_init(databuf, size, config->m_ability);
    _thread_safety_init(databuf);

    databuf->m_is_init = 1;

    return databuf;
}

static inline void _databuf_final(databuf_t *databuf)
{
    databuf->m_is_init = 0;

    pthread_mutex_lock(&databuf->m_thread.m_mutex);
    _thread_safety_final(databuf);
    pthread_mutex_unlock(&databuf->m_thread.m_mutex);

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

int DataBufIsFull(void *handle)
{
    if (!handle) {
        databuf_log("the handle is NULL\n");
        return -1;
    }
    databuf_t *databuf = handle;
    return (databuf->m_remain_size == databuf->m_config.m_size) ? 1 : 0;
}

int DataBufIsEmpty(void *handle)
{
    if (!handle) {
        databuf_log("the handle is NULL\n");
        return -1;
    }
    databuf_t *databuf = handle;
    return (databuf->m_remain_size == 0) ? 1 : 0;
}

int DataBufGetSize(void *handle)
{
    if (!handle) {
        databuf_log("the handle is NULL\n");
        return -1;
    }
    databuf_t *databuf = handle;
    return (databuf->m_config.m_size - databuf->m_remain_size);
}

int DataBufGetRemainSize(void *handle)
{
    if (!handle) {
        databuf_log("the handle is NULL\n");
        return -1;
    }
    databuf_t *databuf = handle;
    return (databuf->m_remain_size);
}

static int _databuf_write(databuf_t *databuf, void *buf, int len)
{
    if (databuf->m_config.m_ability == DATABUF_ABILITY_THREAD_SAFETY) {
        pthread_mutex_lock(&databuf->m_thread.m_mutex);
        /* the databuf can't save the current data */
        while (len > databuf->m_remain_size) {
            struct timespec ts = Time_GetCurDelayMs(INT_MAX);
            int ret;

            databuf->m_thread.m_write_cnt++;
            /* receive the signal and return 0 */
            ret = pthread_cond_timedwait(&databuf->m_thread.m_read_cond, \
                                         &databuf->m_thread.m_mutex, \
                                         &ts);
            databuf->m_thread.m_write_cnt--;

            /* if timed out, discard this data */
            if (ret) {
                pthread_mutex_unlock(&databuf->m_thread.m_mutex);
                return -ret;
            }
        }
    }
    int m_tail = databuf->m_tail;

    if (m_tail + len <= databuf->m_config.m_size) {
        memcpy(databuf->buf + m_tail, buf, len);
    } else {
        int sz = databuf->m_config.m_size - m_tail;
        memcpy(databuf->buf + m_tail, buf, sz);
        memcpy(databuf->buf, buf + sz, len - sz);
    }

    databuf->m_tail        += len;
    databuf->m_tail        %= databuf->m_config.m_size;
    //FIXME 出现负数
    databuf->m_remain_size -= len;

#ifdef DATABUF_DEBUG
    printf("databuf write dump: ");
    _databuf_dump_print(databuf->m_head, \
                        databuf->m_tail, \
                        databuf->buf, \
                        databuf->m_config.m_size);
#endif

    if (databuf->m_config.m_ability == DATABUF_ABILITY_THREAD_SAFETY) {
        if (databuf->m_thread.m_read_cnt) {
            pthread_cond_broadcast(&databuf->m_thread.m_write_cond);
        }
        pthread_mutex_unlock(&databuf->m_thread.m_mutex);
    }

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
        return -2;
    }
    databuf_t *databuf = handle;
    if (databuf->m_is_init == 0) {
        databuf_log("please init databuf first \n");
        return -1;
    }
    return _databuf_write(databuf, buf, len);
}

static int _databuf_read(databuf_t *databuf, void *buf, int len, int peek_flag)
{
    if (databuf->m_config.m_ability == DATABUF_ABILITY_THREAD_SAFETY) {
        pthread_mutex_lock(&databuf->m_thread.m_mutex);
        while (databuf->m_config.m_size - databuf->m_remain_size < len) {
            struct timespec ts = Time_GetCurDelayMs(INT_MAX);
            int ret;

            databuf->m_thread.m_read_cnt++;
            /* receive the signal and return 0 */
            ret = pthread_cond_timedwait(&databuf->m_thread.m_write_cond, \
                                         &databuf->m_thread.m_mutex, \
                                         &ts);
            databuf->m_thread.m_read_cnt--;

            /* if timed out, discard this data */
            if (ret) {
                pthread_mutex_unlock(&databuf->m_thread.m_mutex);
                return -ret;
            }
        }
    }
    int head = databuf->m_head;
    
    if (head + len < databuf->m_config.m_size) {
        memcpy(buf, databuf->buf + head, len);
    } else {
        int sz = databuf->m_config.m_size - head;
        memcpy(buf, databuf->buf + head, sz);
        memcpy(buf + sz, databuf->buf, len - sz);
    }

    if (peek_flag == READ_DATA) {
        databuf->m_head        += len;
        databuf->m_head        %= databuf->m_config.m_size;
        databuf->m_remain_size += len;
    }

#ifdef DATABUF_DEBUG
    printf("databuf read dump: ");
    _databuf_dump_print(databuf->m_head, \
                        databuf->m_tail, \
                        databuf->buf, \
                        databuf->m_config.m_size);
#endif

    if (databuf->m_config.m_ability == DATABUF_ABILITY_THREAD_SAFETY) {
        if (databuf->m_thread.m_write_cnt) {
            pthread_cond_broadcast(&databuf->m_thread.m_read_cond);
        }
        pthread_mutex_unlock(&databuf->m_thread.m_mutex);
    }

    return len;
}

static int _databuf_read_common(void *handle, void *buf, int len, int peek_flag)
{
    if (!handle) {
        databuf_log("the handle is NULL\n");
        return -1;
    }
    if (!buf || len <=0) {
        databuf_log("the buf is NULL or the len less then zero \n");
        return -2;
    }
    databuf_t *databuf = handle;
    if (databuf->m_is_init == 0) {
        databuf_log("please init databuf first \n");
        return -3;
    }
    return _databuf_read(databuf, buf, len, peek_flag);
}

int DataBufRead(void *handle, void *buf, int len)
{
    return _databuf_read_common(handle, buf, len, READ_DATA);
}

int DataBufPeekRead(void *handle, void *buf, int len)
{
    return _databuf_read_common(handle, buf, len, PEEK_READ_DATA);
}

