/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_bitset.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    16/07 2021 11:32
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        16/07 2021      create the file
 * 
 *     last modified: 16/07 2021 11:32
 */
#include <stdio.h>

#include "hy_bitset.h"

#include "hy_assert.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

void HyBitSetSet(char *byte, int index)
{
    HY_ASSERT_NULL_RET(!byte);

    int byte_index = index / 8;
    int bit_index = index % 8;

    byte[byte_index] = byte[byte_index] | 1 << (7 - bit_index);
}

int HyBitSetVal(char *byte, int index)
{
    HY_ASSERT_NULL_RET_VAL(!byte, -1);

    int byte_index = index / 8;
    int bit_index = index % 8;

    return (byte[byte_index] & 1 << (7 - bit_index));
}

