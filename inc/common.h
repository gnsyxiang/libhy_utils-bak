/**
 *
 * Release under GPLv2.
 * 
 * @file    common.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/11 2019 10:27
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/11 2019      create the file
 * 
 *     last modified: 05/11 2019 10:27
 */
#ifndef __LIBUTILS_INC_COMMON_H_
#define __LIBUTILS_INC_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ARRAY_LEN(arr)      (sizeof(arr))
#define ARRAY_ITEM_LEN(arr) (sizeof(arr[0]))
#define ARRAY_CNT(arr)      (int)(ARRAY_LEN(arr) / ARRAY_ITEM_LEN(arr))

#ifdef __cplusplus
}
#endif

#endif

