/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    time_wrapper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/11 2019 10:21
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/11 2019      create the file
 * 
 *     last modified: 06/11 2019 10:21
 */
#include <stdio.h>

#include "time_wrapper.h"

/*
 * 1. struct timeval {
 *        long tv_sec;    //秒
 *        long tv_usec;   //微妙
 *    }
 * 2. struct timespec {
 *        __time_t tv_sec;  //秒
 *        long int tv_nsec; //纳秒
 *    }
 * 3. struct tm {
 *        int tm_set;       //秒钟 - 取值区间为[0,59]
 *        int tm_min;       //分钟 - 取值区间为[0,59]
 *        int tm_hour;      //时钟 - 取值区间为[0,23]
 *        int tm_mday;      //日期 - 取值区间为[1,31]
 *        int tm_mon;       //月份 - 取值区间为[0,11]
 *        int tm_year;      //年份 - 其值从1900开始
 *        int tm_wday;      //星期 - 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推
 *        int tm_yday;      //天数 - 取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推
 *        int tm_isdst;     //夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负。
 *    }
 */

#define BASE_NUM	                (1000)
#define BASE_NUM_2	                (BASE_NUM * BASE_NUM)
#define BASE_NUM_3	                (BASE_NUM * BASE_NUM * BASE_NUM)

#define integet_1000(num)		    ((num) / BASE_NUM)
#define remainder_1000(num)	        ((num) % BASE_NUM)
#define multiplier_1000(num)	    ((num) * BASE_NUM)

#define integet_1000_1000(num)      ((num) / BASE_NUM_2)
#define remainder_1000_1000(num)	((num) % BASE_NUM_2)
#define multiplier_1000_1000(num)	((num) * BASE_NUM_2)

struct timespec Time_GetCurDelayMs(long timeout_ms)
{
    struct timeval tv;
    struct timespec ts;

    /* returns the current time (sec and usec) */
    gettimeofday(&tv, NULL);

    ts.tv_sec = time(NULL) + integet_1000(timeout_ms);
    ts.tv_nsec = multiplier_1000(tv.tv_usec) + BASE_NUM_2 * remainder_1000(timeout_ms);

    ts.tv_sec += ts.tv_nsec / BASE_NUM_3;
    ts.tv_nsec %= BASE_NUM_3;

    return ts;
}

TIME_WRAPPER_EX void Time_SleepUs(long timeout_us)
{
    struct timeval tv;
    tv.tv_sec  = integet_1000_1000(timeout_us);
    tv.tv_usec = remainder_1000_1000(timeout_us);
    select(0, NULL, NULL, NULL, &tv);
}



