/**
 *
 * Release under GPL-3.0.
 * 
 * @file    time_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/11 2019 10:13
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/11 2019      create the file
 * 
 *     last modified: 06/11 2019 10:13
 */
#ifndef __LIBUTILS_INC_TIME_WRAPPER_H_
#define __LIBUTILS_INC_TIME_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <sys/time.h>

#ifndef LIBUTILS_INC_TIME_WRAPPER_GB
#define TIME_WRAPPER_EX extern
#else
#define TIME_WRAPPER_EX
#endif

TIME_WRAPPER_EX struct timespec Time_GetCurDelayMs(long timeout_ms);

TIME_WRAPPER_EX void Time_SleepUs(long timeout_us);

#ifdef __cplusplus
}
#endif

#endif //__LIBUTILS_INC_TIME_WRAPPER_H_

