/**
 *
 * Release under GPLv2.
 * 
 * @file    data_buf.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/11 2019 14:07
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/11 2019      create the file
 * 
 *     last modified: 05/11 2019 14:07
 */
#ifndef __LIBUTILS_INC_DATA_BUF_H_
#define __LIBUTILS_INC_DATA_BUF_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief databuf具备的能力
 */
typedef enum {
    DATABUF_ABILITY_NONE          = 0,          // 最小功能
    DATABUF_ABILITY_BLOCK         = (1 << 0),   // FIXME 暂时没有实现
    DATABUF_ABILITY_THREAD_SAFETY = (1 << 1),   // 线程安全
} DataBufAbility_t;

/**
 * @brief 配置databuf
 * @param m_ability: 参数说明详见DataBufAbility_t
 * @param m_size: 初始化databuf的大小
 *
 * note: m_size在实现中有做4字节对齐
 */
typedef struct {
    DataBufAbility_t m_ability;
    int m_size;
} DataBufConfig_t;

/**
 * @brief 初始化databuf
 *
 * @param databuf_config: 参数说明详见DataBufConfig_t
 *
 * @return 成功返回句柄，失败返回NULL
 */
void *DataBufInit(DataBufConfig_t *databuf_config);

/**
 * @brief 清理databuf使用的资源
 *
 * @param handle: 句柄
 */
void DataBufFinal(void *handle);

/**
 * @brief 向databuf写入数据
 *
 * @param handle: 句柄
 * @param buf: 数据地址
 * @param len: 数据长度
 *
 * @return 成功返回写入的数据长度，失败返回负值
 */
int DataBufWrite(void *handle, void *buf, int len);

/**
 * @brief 从databuf读数据
 *
 * @param handle: 句柄
 * @param buf: 数据地址
 * @param len: 数据长度
 *
 * @return 成功返回读取的数据长度，失败返回负值
 */
int DataBufRead(void *handle, void *buf, int len);

/**
 * @brief 从databuf读数据，但是不取出数据
 *
 * @param handle: 句柄
 * @param buf: 数据地址
 * @param len: 数据长度
 *
 * @return 成功返回读取的数据长度，失败返回负值
 */
int DataBufPeekRead(void *handle, void *buf, int len);

/**
 * @brief 从databuf移除指定的数据
 *
 * @param handle: 句柄
 * @param buf: 数据地址
 * @param len: 数据长度
 *
 * @return 
 */
int DataBufRemoveData(void *handle, void *buf, int len);

/**
 * @brief 获取databuf是否已满
 *
 * @param handle: 句柄
 *
 * @return databuf满返回1，没满返回0
 */
int DataBufIsFull(void *handle);

/**
 * @brief 获取databuf是否为空
 *
 * @param handle: 句柄
 *
 * @return databuf空返回1，没空返回0
 */
int DataBufIsEmpty(void *handle);

/**
 * @brief 获取databuf写入的空间
 *
 * @param handle: 句柄
 *
 * @return 返回实际写入的字节数
 */
int DataBufGetSize(void *handle);

/**
 * @brief 获取databuf剩余存储空间
 *
 * @param handle: 句柄
 *
 * @return 返回databuf剩余的字节数
 */
int DataBufGetRemainSize(void *handle);

/**
 * @brief 清除databuf里面的数据
 *
 * @param handle: 句柄
 */
void DataBufCleanAll(void *handle);

/**
 * @brief 打印databuf里面的数据
 *
 * @param handle: 句柄
 */
void DataBufDump(void *handle);

#ifdef __cplusplus
}
#endif

#endif //__LIBUTILS_INC_DATA_BUF_H_

