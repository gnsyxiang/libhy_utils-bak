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
#ifndef __LIBUTILS_INC_FILE_WRAPPER_H_
#define __LIBUTILS_INC_FILE_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBUTILS_INC_FILE_WRAPPER_GB
#define LIBUTILS_INC_FILE_WRAPPER_EX extern
#else
#define LIBUTILS_INC_FILE_WRAPPER_EX
#endif

#include <sys/types.h>

LIBUTILS_INC_FILE_WRAPPER_EX ssize_t FileWrite(int fd, const void *buf, size_t len);
LIBUTILS_INC_FILE_WRAPPER_EX ssize_t FileRead(int fd, void *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif // __LIBUTILS_INC_FILE_WRAPPER_H_

