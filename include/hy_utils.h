/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_utils.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    04/08 2020 16:05
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        04/08 2020      create the file
 * 
 *     last modified: 04/08 2020 16:05
 */
#ifndef __LIBUTILS_INCLUDE_HY_UTILS_H_
#define __LIBUTILS_INCLUDE_HY_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef void (*HyUtilsProcess_t)(void);

typedef struct {
    void        *buf;
    uint32_t    len;
} BufUnion_t;
#define BUF_UNION_T_LEN (sizeof(BufUnion_t))

BufUnion_t *HyBufUnionCreate(uint32_t len);
void HyBufUnionDestroy(BufUnion_t *buf_union);

void HyIpStr2Int(const char *ip_str, uint32_t *ip_num);
void HyIpInt2Str(uint32_t ip_num, char *ip_str, uint8_t ip_str_len);

#define HY_UTILS_COPY(name, macro, name_len)                                \
    do {                                                                    \
        memset((name), '\0', (name_len));                                   \
        uint32_t str_len = strlen((macro));                                 \
        uint32_t len = (str_len > (name_len)) ? (name_len) - 1 : str_len;   \
        strncpy((name), (macro), len);                                      \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif

