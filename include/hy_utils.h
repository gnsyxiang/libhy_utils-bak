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
#ifndef __LIBHY_UTILS_INCLUDE_HY_UTILS_H_
#define __LIBHY_UTILS_INCLUDE_HY_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdint.h>

#define IP_DOT_LEN (4)
void HyIpStr2Int(const char *ip_str, uint32_t *ip_num);
void HyIpInt2Str(uint32_t ip_num, char *ip_str, uint8_t ip_str_len);

uint32_t HyBinaryStr2Dec(char *binary_str, uint8_t len);
void HyDec2BinaryStr(char *binary_str, uint8_t str_len, uint8_t num, uint32_t dec_num);

void HyHex2Int2Str(char *str, uint8_t str_len, char *addr, uint8_t addr_len);
void HyStr2Int2Hex(char *addr, uint8_t addr_len, char *str, uint8_t str_len);

uint32_t HyUtilsNumTo2N2(uint32_t num);

#define HyUtilsArrayCnt(array) (int)(sizeof((array)) / sizeof((array)[0]))

#define HyUtilsMinMacro(x, y) ((x) < (y) ? (x) : (y))

//判断x是否是2的次方
#define HyUtilsIsPowerOf2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))

#define HY_STRNCPY(dst, src, max_len)   \
do {                                    \
    memset(dst, '\0', max_len);         \
    size_t len = strlen(src);           \
    if (len >= max_len) {               \
        len = max_len - 1;              \
    }                                   \
    strncpy(dst, src, len);             \
} while (0)

#define JUDGE_NULL_RET(param, ret)      \
    if (param) {                        \
        LOGE("the param is NULL \n");   \
        return ret;                     \
    }

#define JUDGE_NULL(param)               \
    if (param) {                        \
        LOGE("the param is NULL \n");   \
        return ;                        \
    }

#define HY_FREE(mem)    \
    do {                \
        free(*mem);     \
        *mem = NULL;    \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif

