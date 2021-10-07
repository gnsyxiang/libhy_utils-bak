/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_assert.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    15/07 2021 15:20
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        15/07 2021      create the file
 * 
 *     last modified: 15/07 2021 15:20
 */
#ifndef __LIBHY_UTILS_INCLUDE_HY_ASSERT_H_
#define __LIBHY_UTILS_INCLUDE_HY_ASSERT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

#define HY_ASSERT(zero) assert(zero);

#define HY_ASSERT_VAL_BREAK(val)        \
    if (val) {                          \
        LOGE("the param is error \n");  \
        break;                          \
    }

#define HY_ASSERT_VAL_RET(val)          \
    if (val) {                          \
        LOGE("the param is error \n");  \
        return ;                        \
    }

#define HY_ASSERT_VAL_RET_VAL(val, ret) \
    if (val) {                          \
        LOGE("the param is error \n");  \
        return ret;                     \
    }

#ifdef __cplusplus
}
#endif

#endif

