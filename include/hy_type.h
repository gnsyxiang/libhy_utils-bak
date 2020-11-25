/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_type.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/08 2020 09:59
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/08 2020      create the file
 * 
 *     last modified: 05/08 2020 09:59
 */
#ifndef __LIBUTILS_INCLUDE_HY_TYPE_H_
#define __LIBUTILS_INCLUDE_HY_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#ifdef __GNUC__
    #define UNPACKED        __attribute__ ((packed))
    #define PACKED_4        __attribute__((aligned (4)))
#else
    #define STRUCT_PACKED
#endif

typedef enum {
    ERR_OK,
    ERR_FAILD,
    ERR_BUSY,
} ErrType_t;

#define ArrayCnt(arr)               (int)(sizeof(arr) / sizeof((arr)[0])) 

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

#define BYTE_BIT(index)     ((uint8_t)(0x1 << index))

#define BYTE_BIT0           BYTE_BIT(0)
#define BYTE_BIT1           BYTE_BIT(1)
#define BYTE_BIT2           BYTE_BIT(2)
#define BYTE_BIT3           BYTE_BIT(3)
#define BYTE_BIT4           BYTE_BIT(4)
#define BYTE_BIT5           BYTE_BIT(5)
#define BYTE_BIT6           BYTE_BIT(6)
#define BYTE_BIT7           BYTE_BIT(7)

#define BYTE_BIT0_1_MASK    (0x03)
#define BYTE_BIT0_2_MASK    (0x07)

#define BYTE_TEST_BIT(state, bit) ((state) & ((0x1) << (bit)))

#ifdef __cplusplus
}
#endif

#endif

