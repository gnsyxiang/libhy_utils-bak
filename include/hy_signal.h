/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_signal.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/12 2020 14:25
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/12 2020      create the file
 * 
 *     last modified: 05/12 2020 14:25
 */
#ifndef __LIBUTILS_INCLUDE_HY_SIGNAL_H_
#define __LIBUTILS_INCLUDE_HY_SIGNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*HySignalHandleFrameCb_t)(void *args);
typedef struct {
    HySignalHandleFrameCb_t     handle_frame_cb;
    void                        *args;
} HySignalHandleCb_t;

void HySignalInit(const char *appname, const char *coredump_path, HySignalHandleCb_t *handle_cb);

#ifdef __cplusplus
}
#endif

#endif

