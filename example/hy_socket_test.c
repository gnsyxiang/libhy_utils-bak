/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_socket_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/12 2020 10:42
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/12 2020      create the file
 * 
 *     last modified: 05/12 2020 10:42
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "hy_socket.h"

#include "hy_log.h"

#ifdef USE_DEBUG
    #define ALONE_DEBUG 1
    #define LOG_CATEGORY_TAG "hy_socket_test"
#endif

int main(int argc, char const* argv[])
{
    HyLogCreate(USE_LOG_LEVEL, 1024);

    HySocketConfig_t socket_config;
    memset(&socket_config, '\0', sizeof(socket_config));
    socket_config.ip = "192.168.1.57";
    socket_config.port = 8888;

    void *socket_handle = HySocketCreate(&socket_config);

    while (1) {
        sleep(1);
        char *buf = "123456789";
        if (-1 == HySocketWrite(socket_handle, buf, strlen(buf))) {
            break;
        }
    }

    HySocketDestroy(socket_handle);

    HyLogDestory();

    return 0;
}
