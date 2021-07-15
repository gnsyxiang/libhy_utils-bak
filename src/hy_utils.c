/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_utils.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hy_utils.h"

#include "hy_log.h"

#define ALONE_DEBUG 1

void HyIpStr2Int(const char *ip_str, uint32_t *ip_num)
{
    if (!ip_str) {
        LOGE("the param is NULL \r\n");
        return;
    }

    int num[IP_DOT_LEN];

	sscanf(ip_str, "%d.%d.%d.%d", &num[0], &num[1], &num[2], &num[3]);

    *ip_num  = 0;
    for (int i = 0; i < IP_DOT_LEN; i++) {
        *ip_num += (num[i] & 0xff) << (8 * (3 - i));
    }
    // printf("ip_str: %s, ip_num: %08x, %u \r\n", ip_str, *ip_num, *ip_num);
}

void HyIpInt2Str(uint32_t ip_num, char *ip_str, uint8_t ip_str_len)
{
    if (!ip_str) {
        LOGE("the param is NULL \r\n");
        return;
    }

    uint8_t *tmp = (uint8_t *)&ip_num;
    uint8_t len = 0;

    for (int i = 0; i < IP_DOT_LEN; i++) {
        len += snprintf(ip_str + len, ip_str_len - len, "%d.", *(tmp + IP_DOT_LEN - 1 - i));
    }
    ip_str[len - 1] = '\0';
    // printf("ip_str: %s, ip_num: %08x, %u \r\n", ip_str, ip_num, ip_num);
}

uint32_t HyBinaryStr2Dec(char *binary_str, uint8_t len)
{
    uint32_t dec_num = 0;
    for (int i = 0; i < len; i++) {
        char buf[2] = {0};
        snprintf(buf, 2, "%c", binary_str[i]);
        // LOGD("buf: %s, %d, %02x \r\n", buf, atoi(buf), atoi(buf) << (len - 1 - i));
        dec_num |= atoi(buf) << (len - 1 - i);
    }
    // LOGD("dec_num: %02x \r\n", dec_num);
    return dec_num;
}

void HyDec2BinaryStr(char *binary_str, uint8_t str_len, uint8_t num, uint32_t dec_num)
{
    uint8_t len = 0;
    memset(binary_str, '\0', str_len);
    for (int i = 0; i < num; i++) {
        if (dec_num & 0x1) {
            len += snprintf(binary_str + len, str_len - len, "1");
        } else {
            len += snprintf(binary_str + len, str_len - len, "0");
        }
        dec_num >>= 1;
    }
}

void HyHex2Int2Str(char *str, uint8_t str_len, char *addr, uint8_t addr_len)
{
    uint8_t len = 0;
    for (int i = 0; i < addr_len; i++) {
        // LOGD("addr: %02x, %d \n", addr[i], (uint8_t)addr[i]);
        len += snprintf(str + len, str_len - len, "%03d", (uint8_t)addr[i]);
    }
}

void HyStr2Int2Hex(char *addr, uint8_t addr_len, char *str, uint8_t str_len)
{
    uint8_t addr_cnt = 0;
    for (int i = 0; i < str_len; i += 3) {
        char buf[4] = {0};
        sscanf(str + i, "%3s", buf);
        uint8_t num = atoi(buf);
        addr[addr_cnt++] = num;
        if (addr_cnt == addr_len) {
            break;
        }
    }
}

uint32_t HyUtilsNumTo2N2(uint32_t num)
{
    uint32_t i = 1;
    uint32_t num_tmp = num;

    while (num >>= 1) {
        i <<= 1;
    }

    return (i < num_tmp) ? i << 1 : i;
}

