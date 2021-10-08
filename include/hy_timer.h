/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_timer.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    16/07 2021 14:25
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        16/07 2021      create the file
 * 
 *     last modified: 16/07 2021 14:25
 */
#ifndef __LIBHY_UTILS_INCLUDE_HY_TIMER_H_
#define __LIBHY_UTILS_INCLUDE_HY_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint32_t slot_num;              ///< 一格周期的总格子数
    uint32_t slot_interval_ms;      ///< 一格子定时的最小刻度
} HyTimerServiceSaveConfig_t;

typedef struct {
    HyTimerServiceSaveConfig_t save_config;
} HyTimerServiceConfig_t;

void HyTimerCreate(HyTimerServiceConfig_t *config);
void HyTimerDestroy(void);

typedef struct {
    size_t expires;
    int32_t repeat_flag;

    void (*timer_cb)(void *args);
    void *args;
} HyTimerConfig_t;

void *HyTimerAdd(HyTimerConfig_t *timer_config);
void HyTimerDel(void *timer_handle);

#ifdef __cplusplus
}
#endif

#endif

