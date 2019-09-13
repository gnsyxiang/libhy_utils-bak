/**
 * 
 * Release under GPLv2.
 * 
 * @file    signal_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    24/07 2019 13:49
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        24/07 2019      create the file
 * 
 *     last modified: 24/07 2019 13:49
 */
#ifndef __LIBUTILS_INC_SIGNAL_WRAPPER_H_
#define __LIBUTILS_INC_SIGNAL_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 
 *
 * @param app_name
 */
void SignalHandleInit(const char * const app_name);

/**
 * @brief 
 */
void SignalHandleFinal(void);

#ifdef __cplusplus
}
#endif

#endif /* __LIBUTILS_INC_SIGNAL_WRAPPER_H_*/

