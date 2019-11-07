/**
 *
 * Release under GPL-3.0.
 * 
 * @file    bitmap_wrapper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/11 2019 20:21
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/11 2019      create the file
 * 
 *     last modified: 06/11 2019 20:21
 */
#ifndef __LIBUTILS_INC_BITMAP_WRAPPER_H_
#define __LIBUTILS_INC_BITMAP_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBUTILS_INC_BITMAP_WRAPPER_GB
#define BITMAP_WTRAPPER_EX extern
#else
#define BITMAP_WTRAPPER_EX
#endif

#define BITMAP_SET_BIT(index)           (0x01 << (index))
#define BITMAP_TEST_BIT(val, index)     ((val) & (BITMAP_SET_BIT(index)))

#define BITMAP_GET_INDEX(val) \
    ({\
        int index = -1; \
        long val_tmp = val; \
        while((val_tmp)) { \
            val_tmp >>= 1; \
            index++; \
        } \
        index; \
     })

#ifdef __cplusplus
}
#endif

#endif //__LIBUTILS_INC_BITMAP_WRAPPER_H_

