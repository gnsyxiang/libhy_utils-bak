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

void *HyFifoCreate(char *buf, uint32_t len);
void HyFifoDestroy(void *handle);

uint32_t HyFifoInsertData(void *handle, char *buf, uint32_t len);
uint32_t HyFifoGetData(void *handle, char *buf, uint32_t len);

void HyFifoDump(void *handle);

#ifdef __cplusplus
}
#endif

#endif

