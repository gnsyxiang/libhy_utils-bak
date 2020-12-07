/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_socket.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/12 2020 09:48
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/12 2020      create the file
 * 
 *     last modified: 05/12 2020 09:48
 */
#ifndef __LIBUTILS_INCLUDE_HY_SOCKET_H_
#define __LIBUTILS_INCLUDE_HY_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct {
    char                *ip;
    uint16_t            port;
    int8_t              non_block_mode;
} HySocketConfig_t;

void *HySocketCreate(HySocketConfig_t *socket_config);
void HySocketDestroy(void *handle);

int HySocketRead(void *handle, char *buf, uint32_t len);
int HySocketWrite(void *handle, const char *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
