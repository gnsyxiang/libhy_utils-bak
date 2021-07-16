/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_timerwheel_rotation.h
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
#ifndef __LIBHY_UTILS_INCLUDE_HY_TIMERWHEEL_ROTATION_H_
#define __LIBHY_UTILS_INCLUDE_HY_TIMERWHEEL_ROTATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint32_t slot_num;
    uint32_t slot_interval;
} HyTimerWheelRotationConfig_t;

void *HyTimerWheelRotationCreate(HyTimerWheelRotationConfig_t *timerwheel_rotation_config);
void HyTimerWheelRotationDestroy(void **handle);

typedef struct {
    size_t expires;
    void (*timer_cb)(void *args);
    void *args;
} HyTimerHandleConfig_t;

void *HyTimerWheelRotationAdd(void *handle, HyTimerHandleConfig_t *timer_config);
void HyTimerWheelRotationDel(void *handle, void *timer_handle);

#ifdef __cplusplus
}
#endif

#endif

