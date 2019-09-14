/**
 *
 * Release under GPLv2.
 * 
 * @file    tiny_things.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    14/09 2019 22:15
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        14/09 2019      create the file
 * 
 *     last modified: 14/09 2019 22:15
 */
#ifndef __LIBUTILS_INC_TINY_THINGS_H_
#define __LIBUTILS_INC_TINY_THINGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define BYTE_ALIGN(len, align) (((len) + (align) - 1) & ~((align) - 1))

#define ALIGN_INT(len)  BYTE_ALIGN(len, sizeof(uint32_t))

#define ALIGN4(len)     BYTE_ALIGN(len, 4)
#define ALIGN3(len)     BYTE_ALIGN(len, 3)
#define ALIGN2(len)     BYTE_ALIGN(len, 2)


#ifdef __cplusplus
}
#endif

#endif // __LIBUTILS_INC_TINY_THINGS_H_

