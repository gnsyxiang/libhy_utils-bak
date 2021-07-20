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

#define HY_ASSERT_NULL_BREAK(param)         \
    if (param) {                            \
        LOGE("the param is NULL \n");       \
        break;                              \
    }

#define HY_ASSERT_NULL_RET(param)           \
    if (param) {                            \
        LOGE("the param is NULL \n");       \
        return ;                            \
    }

#define HY_ASSERT_NULL_RET_VAL(param, ret)  \
    if (param) {                            \
        LOGE("the param is NULL \n");       \
        return ret;                         \
    }

#ifdef __cplusplus
}
#endif

#endif

