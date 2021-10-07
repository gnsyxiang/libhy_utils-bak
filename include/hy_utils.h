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

#include <stdio.h>
#include <stdint.h>

#define IP_INT_DOT_LEN (4)
#define IP_STR_DOT_LEN (16)

/**
 * @brief 把IP字符串转化为整数
 *
 * @param ip_str ip字符串
 * @param ip_num ip整数
 */
void HyIpStr2Int(const char *ip_str, uint32_t *ip_num);

/**
 * @brief 把ip整数转化为ip字符串
 *
 * @param ip_num ip整数
 * @param ip_str ip字符串
 * @param ip_str_len ip字符串的长度
 */
void HyIpInt2Str(uint32_t ip_num, char *ip_str, size_t ip_str_len);

/**
 * @brief hex转化成整数，然后再转化成字符串
 *
 * @param addr 需要转换的hex数据地址
 * @param addr_len 需要转化的长度
 * @param str 转化成功后的字符串
 * @param str_len 字符串的长度
 */
void HyHex2Int2Str(char *addr, size_t addr_len, char *str, size_t str_len);

/**
 * @brief 字符串转化成整数，然后在转化成hex
 *
 * @param str 需要转化的字符串
 * @param str_len 字符串的长度
 * @param addr 转化成功后的hex数据地址
 * @param addr_len hex数据地址的长度
 */
void HyStr2Int2Hex(char *str, size_t str_len, char *addr, size_t addr_len);

/**
 * @brief 把比特数组转化成整数
 *
 * @param binary_str 比特数组
 * @param len 长度
 *
 * @return 返回整数
 */
uint32_t HyBitStr2Dec(char *binary_str, size_t len);

/**
 * @brief 把整数转化成bit数组
 *
 * @param num 整数
 * @param num_len 整数个数
 * @param bit_str bit数组地址
 * @param str_len bit数组长度
 */
void HyDec2BitStr(uint32_t num, size_t num_len, char *bit_str, size_t str_len);

/**
 * @brief 写指定长度的数据
 *
 * @param fd 句柄
 * @param buf 数据首地址
 * @param len 数据长度
 *
 * @return 成功返回len指定的长度，失败返回-1
 */
int32_t HyUtilsWrite(int32_t fd, const void *buf, size_t len);

/**
 * @brief 读指定成都的数据
 *
 * @param fd 句柄
 * @param buf 数据首地址
 * @param len 数据长度
 *
 * @return 成功返回len指定的长度，失败返回-1
 */
int32_t HyUtilsRead(int32_t fd, void *buf, size_t len);

#define HyUtilsArrayCnt(array) (int32_t)(sizeof((array)) / sizeof((array)[0]))

#define HyUtilsMinMacro(x, y) ((x) < (y) ? (x) : (y))

#define HyUtilsIsPowerOf2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))

#ifdef __cplusplus
}
#endif

#endif

