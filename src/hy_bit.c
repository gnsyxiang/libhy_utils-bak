/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_bit.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    07/10 2021 14:53
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        07/10 2021      create the file
 * 
 *     last modified: 07/10 2021 14:53
 */
#include <stdio.h>

#include "hy_bit.h"

#include "hy_assert.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

void HyBitSet(char *byte, uint32_t index)
{
    HY_ASSERT_VAL_RET(!byte);

    int byte_index = index / 8;
    int bit_index = index % 8;

    byte[byte_index] = byte[byte_index] | 1 << (7 - bit_index);
}

int32_t HyBitVal(char *byte, uint32_t index)
{
    HY_ASSERT_VAL_RET_VAL(!byte, -1);

    int byte_index = index / 8;
    int bit_index = index % 8;

    return (byte[byte_index] & 1 << (7 - bit_index));
}

