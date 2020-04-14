/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    md5sum.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    14/04 2020 16:32
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        14/04 2020      create the file
 * 
 *     last modified: 14/04 2020 16:32
 */
#include "hal/hal_string.h"

#include "utils_md5sum.h"

int main(int argc, const char *argv[])
{
    hal_uint8_t digest[16] = {0};
    hal_uint8_t md5sum[16] = {
        0xe8, 0x07, 0xf1, 0xfc, 0xf8, 0x2d, 0x13, 0x2f,
        0x9b, 0xb0, 0x18, 0xca, 0x67, 0x38, 0xa1, 0x9f
    };
    hal_uint8_t msg[11] = {"1234567890"};

    printf("Encrypted data: %s \n", msg);

    UtilsMd5sum(msg, Hal_strlen((char *)msg), digest);

    for (hal_int32_t i = 0; i < 16; i++) {
        printf("%2.2x", digest[i]);
    }
    printf("\n");

    printf("result: %s \n", \
            Hal_memcmp(digest, md5sum, 16) == 0 ? "successful" : "failed");

    return 0;
}
