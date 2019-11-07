/**
 *
 * Release under GPL-3.0.
 * 
 * @file    core_dump.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/11 2019 17:09
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/11 2019      create the file
 * 
 *     last modified: 06/11 2019 17:09
 */
#ifndef __LIBUTILS_INC_CORE_DUMP_H_
#define __LIBUTILS_INC_CORE_DUMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBUTILS_INC_CORE_DUMP_GB
#define CORE_DUMP_EX extern
#else
#define CORE_DUMP_EX
#endif

CORE_DUMP_EX void Core_OpenDump(void);

#ifdef __cplusplus
}
#endif

#endif //__LIBUTILS_INC_CORE_DUMP_H_

