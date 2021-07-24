/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_math.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    01/04 2021 15:31
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        01/04 2021      create the file
 * 
 *     last modified: 01/04 2021 15:31
 */
#ifndef __LIBHY_UTILS_INCLUDE_HY_MATH_H_
#define __LIBHY_UTILS_INCLUDE_HY_MATH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_type.h"

/**
 * @brief 32位无符号最小值
 */
#ifndef HY_UINT32_MIN
#define HY_UINT32_MIN (0x00000000)
#endif /* ifndef HY_UINT32_MIN */

/**
 * @brief 32位无符号最大值
 */
#ifndef HY_UINT32_MAX
#define HY_UINT32_MAX (0xffffffff)
#endif /* ifndef HY_UINT32_MAX */

/**
 * @brief 32位有符号最小值
 */
#ifndef HY_INT32_MIN
#define HY_INT32_MIN (-2147483647 - 1) /**< 0x80000000 */
#endif /* ifndef HY_INT32_MIN */

/**
 * @brief 32位有符号最大值
 */
#ifndef HY_INT32_MAX
#define HY_INT32_MAX (0x7fffffff)
#endif /* ifndef HY_INT32_MAX */

/**
 * @brief 判断x是否是2的次方
 */
#define IS_POWER_OF_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))

/* 
 * BYTE_ALIGN:  字节对齐
 * ALIGN4:      4字节对齐
 * ALIGN2:      2字节对齐
 * ALIGN4_UP:   绝对字节对齐(原来已经事4字节对齐，使用后再增加4个字节)
 */
#define BYTE_ALIGN(len, align)  (((len) + (align) - 1) & ~((align) - 1))
#define ALIGN4(len)             BYTE_ALIGN(len, 4)
#define ALIGN2(len)             BYTE_ALIGN(len, 2)
#define ALIGN4_UP(len)          (BYTE_ALIGN(len, 4) + ALIGN4(1))

/**
 * @brief 产生随机数
 *
 * @param range: 随机数范围0 - range
 */
hy_u32_t HyMathGenerateRandom(hy_u32_t range);

#ifdef __cplusplus
}
#endif

#endif

