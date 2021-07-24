/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_type.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    15/07 2021 08:54
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 *
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        15/07 2021      create the file
 * 
 *     last modified: 15/07 2021 08:54
 */
#ifndef __LIBHY_UTILS_INCLUDE_HY_TYPE_H_
#define __LIBHY_UTILS_INCLUDE_HY_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef char                hy_s8_t;
typedef short               hy_s16_t;
typedef int                 hy_s32_t;
typedef long long           hy_s64_t;

typedef unsigned char       hy_u8_t;
typedef unsigned short      hy_u16_t;
typedef unsigned int        hy_u32_t;
typedef unsigned long long  hy_u64_t;

typedef char                hy_char_t;
typedef double              hy_double_t;

#ifndef NULL
#define NULL (void *)0
#endif

#ifdef __cplusplus
}
#endif

#endif

