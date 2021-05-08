/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_compile.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    01/04 2021 15:49
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        01/04 2021      create the file
 * 
 *     last modified: 01/04 2021 15:49
 */
#ifndef __LIBHY_UTILS_INCLUDE_HY_COMPILE_H_
#define __LIBHY_UTILS_INCLUDE_HY_COMPILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__
    #ifdef BUILDING_DLL
        #ifdef __GNUC__
            #define EXPORT_FUNC __attribute__((dllexport))
        #else
            #define EXPORT_FUNC __declspec(dllexport)
        #endif
    #else
        #ifdef __GNUC__
            #define EXPORT_FUNC __attribute__((dllimport))
        #else
            #define EXPORT_FUNC __declspec(dllimport)
        #endif
    #endif
#else
    #ifdef __GNUC__
        #if __GNUC__ >= 4 || defined(__arm__)
            #define EXPORT_FUNC __attribute__((visibility("default")))
            // #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
        #else
            #error "gcc version too low"
        #endif
    #endif
#endif

#ifdef __cplusplus
}
#endif

#endif

