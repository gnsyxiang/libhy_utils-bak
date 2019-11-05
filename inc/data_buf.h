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

typedef enum {
    DATABUF_ABILITY_WRITE         = (1 << 0),
    DATABUF_ABILITY_READ          = (1 << 1),
    DATABUF_ABILITY_DISCARD_DATA  = (1 << 2),
    DATABUF_ABILITY_THREAD_SAFETY = (1 << 3),
} DataBufAbility_t;

typedef struct {
    DataBufAbility_t m_ability;
    int m_size;
} DataBufConfig_t;

void *DataBufInit(DataBufConfig_t *databuf_config);
void DataBufFinal(void *handle);

int DataBufWrite(void *handle, void *buf, int len);
int DataBufRead(void *handle, void *buf, int len);
int DataBufPeekRead(void *handle, void *buf, int len);

int DataBufIsFull(void *handle);
int DataBufIsEmpty(void *handle);

void DataBufDump(void *handle);

#ifdef __cplusplus
}
#endif

#endif //__LIBUTILS_INC_DATA_BUF_H_

