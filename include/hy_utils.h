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
    char        *buf;
    uint32_t    len;
} BufUnion_t;
#define BUF_UNION_T_LEN (sizeof(BufUnion_t))

BufUnion_t *HyBufUnionCreate(uint32_t len);
void HyBufUnionDestroy(BufUnion_t *buf_union);

#define IP_DOT_LEN (4)
void HyIpStr2Int(const char *ip_str, uint32_t *ip_num);
void HyIpInt2Str(uint32_t ip_num, char *ip_str, uint8_t ip_str_len);

uint32_t HyBinaryStr2Dec(char *binary_str, uint8_t len);
void HyDec2BinaryStr(char *binary_str, uint8_t str_len, uint8_t num, uint32_t dec_num);

void HyHex2Int2Str(char *str, uint8_t str_len, char *addr, uint8_t addr_len);

uint32_t HyUtilsNumTo2N2(uint32_t num);

#define HY_UTILS_COPY(name, macro, name_len)        \
    do {                                            \
        memset((name), '\0', (name_len));           \
        strncpy((name), (macro), name_len - 1);     \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif

