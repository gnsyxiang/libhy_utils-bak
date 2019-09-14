/**
 * 
 * Release under GPLv2.
 * 
 * @file    ringbuf.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    09/05 2018 14:13
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        09/05 2018      create the file
 * 
 *     last modified: 09/05 2018 14:13
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#include "tiny_things.h"
#include "ringbuf.h"

#define rb_log(fmt, ...) \
    printf("<%s:%d, result: %s> " fmt, \
            __func__, __LINE__, strerror(errno), ##__VA_ARGS__);

// #include "typedef_helper.h"
struct ringbuf {
    uint32_t head;
    uint32_t tail;

    uint32_t size;
    uint32_t remain_size;

    pthread_mutex_t mutex;
    pthread_cond_t in_cond;
    pthread_cond_t out_cond;

    uint32_t in_sleep_cnt;
    uint32_t out_sleep_cnt;

    char buf[0];                // 不用指针，如果是协议的话，方便整包发送
};
#define RINGBUF_LEN     (sizeof(struct ringbuf))


//FIXME 合并到timer_wrapper.h
#if 1
#define BASE_NUM	(1000)
#define BASE_NUM_2	(1000 * 1000)
#define BASE_NUM_3	(1000 * 1000 * 1000)

#define take_integet_1000(num)		((num) / BASE_NUM)
#define take_remainder_1000(num)	((num) % BASE_NUM)
#define take_multiplier_1000(num)	((num) * BASE_NUM)

#include <sys/time.h>

static struct timespec cur_delay_ms(unsigned int timeout_ms)
{
    struct timeval tv;
    struct timespec ts;

    /* returns the current time (sec and usec) */
    gettimeofday(&tv, NULL);

    ts.tv_sec = time(NULL) + take_integet_1000(timeout_ms);
    ts.tv_nsec = take_multiplier_1000(tv.tv_usec)
        + BASE_NUM_2 * take_remainder_1000(timeout_ms);

    ts.tv_sec += ts.tv_nsec / BASE_NUM_3;
    ts.tv_nsec %= BASE_NUM_3;

    return ts;
}
#endif

prb_t rb_init(uint32_t size)
{
    size = ALIGN4(size);
    prb_t rb = calloc(1, RINGBUF_LEN + size);
    if (!rb) {
        rb_log("calloc faild");
        return NULL;
    }
    
    rb->head = 0;
    rb->tail = 0;

    rb->size = size;
    rb->remain_size = size;

    pthread_mutex_init(&rb->mutex, NULL);
    pthread_cond_init(&rb->in_cond, NULL);
    pthread_cond_init(&rb->out_cond, NULL);

    return rb;
}

void rb_clean(prb_t rb)
{
    if (!rb) {
        rb_log("ringbuf is NULL");
        return;
    }

    pthread_mutex_destroy(&rb->mutex);
    pthread_cond_destroy(&rb->in_cond);
    pthread_cond_destroy(&rb->out_cond);

    free(rb);
}

int32_t rb_in_timeout(prb_t rb, void *buf, uint32_t size, uint32_t timeout_ms)
{
    int32_t tail;

    pthread_mutex_lock(&rb->mutex);

    /* the ringbuf can't save the current data */
    while (size > rb->remain_size) {
        struct timespec ts = cur_delay_ms(timeout_ms);
        int32_t ret;

        rb->in_sleep_cnt++;
        /* receive the signal and return 0 */
        ret = pthread_cond_timedwait(&rb->out_cond, &rb->mutex, &ts);
        rb->in_sleep_cnt--;

        /* if timed out, discard this data */
        if (ret) {
            pthread_mutex_unlock(&rb->mutex);
            return -ret;
        }
    }

    tail = rb->tail;

    if (tail + size < rb->size) {
        memcpy(rb->buf + tail, buf, size);
        rb->tail = tail + size;
    } else {
        int32_t sz = rb->size - tail;

        memcpy(rb->buf + tail, buf, sz);
        memcpy(rb->buf, buf + sz, size - sz);

        rb->tail = size - sz;
    }

    if (rb->remain_size > 0) {
        rb->remain_size -= size;
    }

    if (rb->out_sleep_cnt) {
        pthread_cond_broadcast(&rb->in_cond);
    }

    pthread_mutex_unlock(&rb->mutex);

    return size;
}

static int32_t rb_out_timeout_base(
        prb_t rb,
        void *buf,
        uint32_t size,
        uint32_t timeout_ms,
        int32_t del_data_flag)
{
    int head;

    pthread_mutex_lock(&rb->mutex);

    /* printf("size: %d, remain_size: %d \n", rb->size, rb->remain_size); */

    /* there is not enough data in the ringbuf */
    while (rb->size - rb->remain_size < size) {
        struct timespec ts = cur_delay_ms(timeout_ms);
        int ret;

        rb->out_sleep_cnt++;
        /* receive the signal and return 0 */
        ret = pthread_cond_timedwait(&rb->in_cond, &rb->mutex, &ts);    
        rb->out_sleep_cnt--;

        /* if timed out, discard this data */
        if (ret) {
            pthread_mutex_unlock(&rb->mutex);
            return -ret;
        }
    }

    head = rb->head;

    if (head + size < rb->size) {
        memcpy(buf, rb->buf + head, size);

        if (del_data_flag)
            rb->head = head + size;
    } else {
        int sz = rb->size - head;

        memcpy(buf, rb->buf + head, sz);
        memcpy(buf + sz, rb->buf, size - sz);

        if (del_data_flag)
            rb->head = size - sz;
    }

    if (del_data_flag) {
        if (rb->remain_size < rb->size) {
            rb->remain_size += size;
        }

        if (rb->in_sleep_cnt) {
            pthread_cond_broadcast(&rb->out_cond);
        }
    }

    pthread_mutex_unlock(&rb->mutex);

    return size;
}

int32_t rb_out_timeout(prb_t rb, void *buf, uint32_t size, uint32_t timeout_ms)
{
    return rb_out_timeout_base(rb, buf, size, timeout_ms, 1);
}

int32_t rb_out_peek_timeout(prb_t rb, void *buf, uint32_t size, uint32_t timeout_ms)
{
    return rb_out_timeout_base(rb, buf, size, timeout_ms, 0);
}

int32_t rb_remove(prb_t rb, uint32_t size)
{
    pthread_mutex_lock(&rb->mutex);

    if (size < rb->size - rb->remain_size) {
        rb->head = (rb->head + size) % rb->size;
        rb->remain_size += size;
    } else {
        size = rb->size - rb->remain_size;
        rb->head = rb->tail;
        rb->remain_size = rb->size;
    }

    if (rb->in_sleep_cnt) {
        pthread_cond_broadcast(&rb->out_cond);
    }

    pthread_mutex_unlock(&rb->mutex);

    return size;
}

uint32_t rb_is_empty(prb_t rb)
{
    return (rb->remain_size == rb->size)? 1 : 0;
}

uint32_t rb_is_full(prb_t rb)
{
    return (rb->remain_size == 0)? 1 : 0;
}

uint32_t rb_remain_size(prb_t rb)
{
    return rb->remain_size;
}

