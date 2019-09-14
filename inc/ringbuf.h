/**
 * 
 * Release under GPLv2.
 * 
 * @file    ringbuf.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    09/05 2018 14:15
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        09/05 2018      create the file
 * 
 *     last modified: 09/05 2018 14:15
 */
#ifndef __LIBUTILS_INC_RINGBUF_H_
#define __LIBUTILS_INC_RINGBUF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <limits.h>
#include <stdint.h>

typedef struct ringbuf* prb_t;

/**
 * @brief ringbuf init
 *
 * @param size: the ringbuf size
 *
 * @return ringbuf handler for success, otherwise NULL
 */
prb_t rb_init(uint32_t size);

/**
 * @brief ringbuf clean resources
 *
 * @param rb: the ringbuf handler
 *
 * @return none
 */
void rb_clean(prb_t rb);

/**
 * @brief judge the ringbuf whether it is empty
 *
 * @param rb: the ringbuf handler
 *
 * @return one for empty, zero for hasing data
 */
uint32_t rb_is_empty(prb_t rb);

/**
 * @brief judge the ringbuf whether it is full
 *
 * @param rb: the ringbuf handler
 *
 * @return one for full, zero for 
 */
uint32_t rb_is_full(prb_t rb);

/**
 * @brief return the ringbuf remain size
 *
 * @param rb: the ringbuf handler
 *
 * @return the remain size of ringbuf
 */
uint32_t rb_remain_size(prb_t rb);

/**
 * @brief put data into ringbuf
 *
 * @param rb: the ringbuf handler
 * @param buf: the input data
 * @param size: the data size
 * @param timeout_ms: timeout ms
 *
 * @return: the input data 
 */
int32_t rb_in_timeout(prb_t rb, void *buf, uint32_t size, uint32_t timeout_ms);

/**
 * @brief 
 *
 * @param pringbuf
 * @param buf
 * @param size
 *
 * @return 
 */
#define rb_in(pringbuf, buf, size) \
        rb_in_timeout(pringbuf, buf, size, INT_MAX)

/**
 * @brief get data from ringbuf
 *
 * @param rb
 * @param buf
 * @param size
 * @param timeout_ms
 *
 * @return 
 */
int32_t rb_out_timeout(prb_t rb, void *buf, uint32_t size, uint32_t timeout_ms);


/**
 * @brief 
 *
 * @param pringbuf
 * @param buf
 * @param size
 *
 * @return 
 */
#define rb_out(pringbuf, buf, size) \
        rb_out_timeout(pringbuf, buf, size, INT_MAX)

/**
 * @brief 
 *
 * @param rb
 * @param buf
 * @param size
 * @param timeout_ms
 *
 * @return 
 */
int32_t rb_out_peek_timeout(prb_t rb, void *buf, uint32_t size, uint32_t timeout_ms);

/**
 * @brief 
 *
 * @param pringbuf
 * @param buf
 * @param size
 *
 * @return 
 */
#define rb_out_peek(pringbuf, buf, size) \
        rb_out_peek_timeout(pringbuf, buf, size, INT_MAX)

/**
 * @brief 
 *
 * @param rb
 * @param size
 *
 * @return 
 */
int32_t rb_remove(prb_t rb, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* __LIBUTILS_INC_RINGBUF_H_ */

