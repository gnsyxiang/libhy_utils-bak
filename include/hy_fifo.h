/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_fifo.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    23/09 2020 16:01
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        23/09 2020      create the file
 * 
 *     last modified: 23/09 2020 16:01
 */
#ifndef __LIBUTILS_INCLUDE_HY_FIFO_H_
#define __LIBUTILS_INCLUDE_HY_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief 获取FIFO各个长度
 */
typedef enum {
    HY_FIFO_TOTAL_LEN,
    HY_FIFO_USED_LEN,
    HY_FIFO_FREE_LEN,
} HyFifoInfoType_t;

/**
 * @brief 创建fifo
 *
 * @param handle: 操作fifo句柄
 * @param buf: 存储fifo数据空间
 * @param len: 存储fifo数据空间长度
 */

void *HyFifoCreate(uint32_t len);

/**
 * @brief 销毁fifo
 *
 * @param handle: 操作fifo句柄
 */
void HyFifoDestroy(void *handle);

/**
 * @brief 打印fifo
 *
 * @param handle: 操作fifo句柄
 */
void HyFifoDump(void *handle);

/**
 * @brief 清除fifo中的数据
 *
 * @param handle: 操作fifo句柄
 */
void HyFifoClean(void *handle);

/**
 * @brief 获取FIFO各个长度
 *
 * @param handle: 操作fifo句柄
 * @param type: 获取具体类型的长度，参考HyFifoInfoType_t
 *
 * @return 返回大小
 */
int32_t HyFifoGetInfo(void *handle, HyFifoInfoType_t type);

/**
 * @brief 向fifo中插入数据
 *
 * @param handle: 操作fifo句柄
 * @param buf: 插入的数据
 * @param len: 插入数据的大小
 *
 * @return 返回成功插入的字节数
 */
uint32_t HyFifoInsertData(void *handle, const char *buf, uint32_t len);

/**
 * @brief 从fifo中删除数据
 *
 * @param handle: 操作fifo句柄
 * @param len: 删除数据的长度
 *
 * @return 
 */
uint32_t HyFifoUpdateTail(void *handle, uint32_t len);

/**
 * @brief 从fifo中获取数据
 *
 * @param handle: 操作fifo句柄
 * @param buf: 被取出的数据
 * @param len: 被取出数据的大小
 *
 * @return 返回成功取出的数据
 */
uint32_t HyFifoGetData(void *handle, const char *buf, uint32_t len);

/**
 * @brief 从fifo中获取数据
 *
 * @param handle: 操作fifo句柄
 * @param buf: 被取出的数据
 * @param len: 被取出数据的大小
 *
 * @return 返回成功取出的数据
 *
 * note: 该操作不会删除数据
 */
uint32_t HyFifoPeekData(void *handle, const char *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif

