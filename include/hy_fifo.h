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
    HY_FIFO_INFO_LEN_TOTAL,
    HY_FIFO_INFO_LEN_USED,
    HY_FIFO_INFO_LEN_FREE,
} HyFifoInfoType_t;

/**
 * @brief 创建fifo
 *
 * @param buf: 存储fifo数据空间
 * @param len: 存储fifo数据空间长度
 *
 * @return 返回操作fifo句柄
 *
 * note: buf空间需要使用者维护的原因:
 * 1, 使用者知道该空间何时创建，何时释放
 * 2，对于单片机系统没有堆的概念，不能在内部使用malloc创建空间
 */
void *HyFifoCreate(char *buf, uint32_t len);

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
 * @brief 获取FIFO各个长度
 *
 * @param handle: 操作fifo句柄
 * @param type: 获取具体类型的长度
 *
 * @return 返回大小
 */
uint32_t HyFifoGetInfo(void *handle, HyFifoInfoType_t type);

/**
 * @brief 向fifo中插入数据
 *
 * @param handle: 操作fifo句柄
 * @param buf: 插入的数据
 * @param len: 插入数据的大小
 *
 * @return 返回成功插入的字节数
 */
uint32_t HyFifoInsertData(void *handle, char *buf, uint32_t len);

/**
 * @brief 从fifo中获取数据
 *
 * @param handle: 操作fifo句柄
 * @param buf: 被取出的数据
 * @param len: 被取出数据的大小
 *
 * @return 返回成功取出的数据
 */
uint32_t HyFifoGetData(void *handle, char *buf, uint32_t len);

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
uint32_t HyFifoPeekData(void *handle, char *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif

