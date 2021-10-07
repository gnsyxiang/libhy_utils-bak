/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_dlopen.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    10/09 2021 09:06
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        10/09 2021      create the file
 * 
 *     last modified: 10/09 2021 09:06
 */
#ifndef __LIBHY_UTILS_INCLUDE_HY_DLOPEN_H_
#define __LIBHY_UTILS_INCLUDE_HY_DLOPEN_H_

#ifdef __cplusplus
extern "C" {
#endif

void *HyDlLibOpen(const char *so_name);
void HyDlLibClose(void **handle);

void *HyDlLibLoadSymbol(void *handle, const char *symbol);

#ifdef __cplusplus
}
#endif

#endif

