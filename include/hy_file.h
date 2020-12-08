/**
 *
 * Release under GPL-3.0.
 * 
 * @file    file_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    21/11 2019 19:32
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        21/11 2019      create the file
 * 
 *     last modified: 21/11 2019 19:32
 */
#ifndef __LIBUTILS_INCLUDE_HY_FILE_H_
#define __LIBUTILS_INCLUDE_HY_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "hy_utils.h"

uint32_t HyFileWrite(int fd, const void *buf, uint32_t len);
uint32_t HyFileRead(int fd, void *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif // __LIBUTILS_INC_FILE_WRAPPER_H_

