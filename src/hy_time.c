/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_time.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    27/11 2020 08:11
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        27/11 2020      create the file
 * 
 *     last modified: 27/11 2020 08:11
 */
#include <stdio.h>
#include <sys/time.h>
#include <errno.h>

#include "hy_time.h"

#define BASE_NUM	(1000)
#define BASE_NUM_2	(1000 * 1000)
#define BASE_NUM_3	(1000 * 1000 * 1000)

#define take_integet_1000(num)		((num) / BASE_NUM)
#define take_remainder_1000(num)	((num) % BASE_NUM)
#define take_multiplier_1000(num)	((num) * BASE_NUM)

time_t HyTimeGetTickMs(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return (take_multiplier_1000(tv.tv_sec) + take_integet_1000(tv.tv_usec));
}

void HyTimeDelayUs(uint32_t us)
{
    struct timeval tv;
    tv.tv_sec   = 0;
    tv.tv_usec  = us;

    int err;
    do {
        err = select(0, NULL, NULL, NULL, &tv);
    } while(err < 0 && errno == EINTR);
}

