/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_bitset.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    16/07 2021 11:29
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        16/07 2021      create the file
 * 
 *     last modified: 16/07 2021 11:29
 */
#ifndef __LIBHY_UTILS_INCLUDE_HY_BITSET_H_
#define __LIBHY_UTILS_INCLUDE_HY_BITSET_H_

#ifdef __cplusplus
extern "C" {
#endif

void HyBitSetSet(char *byte, int index);

int HyBitSetVal(char *byte, int index);

#ifdef __cplusplus
}
#endif

#endif

