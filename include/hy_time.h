/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_time.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    27/11 2020 08:08
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        27/11 2020      create the file
 * 
 *     last modified: 27/11 2020 08:08
 */
#ifndef __LIBUTILS_INCLUDE_HY_TIME_H_
#define __LIBUTILS_INCLUDE_HY_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

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

#include <stdint.h>
#include <time.h>

#define HY_TIME_1S_FACTOR       (1000)

time_t HyTimeGetTickMs(void);

#ifdef __cplusplus
}
#endif

#endif

