/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_utils.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    04/08 2020 16:05
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        04/08 2020      create the file
 * 
 *     last modified: 04/08 2020 16:05
 */
#ifndef __LIBHY_UTILS_INCLUDE_HY_UTILS_H_
#define __LIBHY_UTILS_INCLUDE_HY_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define HyUtilsArrayCnt(array) (int32_t)(sizeof((array)) / sizeof((array)[0]))

#define HyUtilsMinMacro(x, y) ((x) < (y) ? (x) : (y))

#define HyUtilsIsPowerOf2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))

#ifdef __cplusplus
}
#endif

#endif

