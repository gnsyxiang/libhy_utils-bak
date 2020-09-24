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
 * @brief fifo结构体
 *
 * note: 外部最好不好直接操作该结构体，最好使用API函数
 */
typedef struct {
    uint32_t    cnt;
    uint32_t    head;
    uint32_t    tail;

    uint32_t    len;
    char        *buf;
} HyFifoContext_t;
#define HY_FIFO_CONTEXT_T_LEN (sizeof(HyFifoContext_t))

/**
 * @brief 创建fifo
 *
 * @param context: 操作fifo句柄
 * @param buf: 存储fifo数据空间
 * @param len: 存储fifo数据空间长度
 *
 * note: buf和context空间需要使用者维护的原因:
 * 1, 使用者知道该空间何时创建，何时释放
 * 2，对于单片机系统没有堆的概念，不能在内部使用malloc
 */

void HyFifoCreate(HyFifoContext_t *context, char *buf, uint32_t len);

/**
 * @brief 销毁fifo
 *
 * @param context: 操作fifo句柄
 */
void HyFifoDestroy(HyFifoContext_t *context);

/**
 * @brief 打印fifo
 *
 * @param context: 操作fifo句柄
 */
void HyFifoDump(HyFifoContext_t *context);

/**
 * @brief 清除fifo中的数据
 *
 * @param context: 操作fifo句柄
 */
void HyFifoClean(HyFifoContext_t *context);

/**
 * @brief 获取FIFO各个长度
 *
 * @param context: 操作fifo句柄
 * @param type: 获取具体类型的长度，参考HyFifoInfoType_t
 *
 * @return 返回大小
 */
int32_t HyFifoGetInfo(HyFifoContext_t *context, HyFifoInfoType_t type);

/**
 * @brief 向fifo中插入数据
 *
 * @param context: 操作fifo句柄
 * @param buf: 插入的数据
 * @param len: 插入数据的大小
 *
 * @return 返回成功插入的字节数
 */
uint32_t HyFifoInsertData(HyFifoContext_t *context, char *buf, uint32_t len);

/**
 * @brief 从fifo中获取数据
 *
 * @param context: 操作fifo句柄
 * @param buf: 被取出的数据
 * @param len: 被取出数据的大小
 *
 * @return 返回成功取出的数据
 */
uint32_t HyFifoGetData(HyFifoContext_t *context, char *buf, uint32_t len);

/**
 * @brief 从fifo中获取数据
 *
 * @param context: 操作fifo句柄
 * @param buf: 被取出的数据
 * @param len: 被取出数据的大小
 *
 * @return 返回成功取出的数据
 *
 * note: 该操作不会删除数据
 */
uint32_t HyFifoPeekData(HyFifoContext_t *context, char *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif

