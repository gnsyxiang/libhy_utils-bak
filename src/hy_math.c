/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_math.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    10/05 2021 17:05
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        10/05 2021      create the file
 * 
 *     last modified: 10/05 2021 17:05
 */
#include <stdio.h>
#include<stdlib.h>
#include<time.h>

#include "hy_math.h"

#include "hy_compile.h"
#include "hy_log.h"

#define ALONE_DEBUG 1
#define LOG_CATEGORY_TAG "hy_math"

WEAK hy_uint64_t HyTimeGetCurrentTime2Us(void)
{
    LOGE("please link libhy_hal.so \n");
    return 0;
}

hy_uint32_t HyMathGenerateRandom(hy_uint32_t range)
{
    srand(HyTimeGetCurrentTime2Us());
    return rand() % range;
}
