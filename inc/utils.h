/**
 *
 * Release under GPLv2.
 * 
 * @file    utils.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    14/09 2019 23:51
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        14/09 2019      create the file
 * 
 *     last modified: 14/09 2019 23:51
 */
#ifndef __LIBUTILS_INC_UTILS_H_
#define __LIBUTILS_INC_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBUTILS_INC_UTILS_GB
#define LIBUTILS_INC_UTILS_EX extern
#else
#define LIBUTILS_INC_UTILS_EX
#endif

#include <sys/types.h>

#define BYTE_ALIGN(len, align)  (((len) + (align) - 1) & ~((align) - 1))
#define ALIGN4(len)             BYTE_ALIGN(len, 4)
#define ALIGN3(len)             BYTE_ALIGN(len, 3)
#define ALIGN2(len)             BYTE_ALIGN(len, 2)

#define ARRAY_LEN(arr)          (sizeof(arr))
#define ARRAY_ITEM_LEN(arr)     (sizeof(arr[0]))
#define ARRAY_CNT(arr)          (int)(ARRAY_LEN(arr) / ARRAY_ITEM_LEN(arr))

#define DATA_TYPE_LEN(type)     (sizeof(type))

LIBUTILS_INC_UTILS_EX void DumpHexData(char *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif // __LIBUTILS_INC_UTILS_H_

