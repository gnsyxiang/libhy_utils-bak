/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_socket.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/12 2020 09:50
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/12 2020      create the file
 * 
 *     last modified: 05/12 2020 09:50
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "hy_socket.h"

#include "hy_type.h"
#include "hy_log.h"

#ifdef USE_DEBUG
    #define ALONE_DEBUG 1
    #define LOG_CATEGORY_TAG "hy_socket"
#endif

typedef struct {
    int                 fd;
} socket_context_t;

static int _socket_init(socket_context_t *context, HySocketConfig_t *socket_config)
{
    context->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (context->fd < 0) {
        LOGE("socket faild \n");
        return ERR_FAILD;
    }

    struct sockaddr_in    servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(socket_config->port);
    if (inet_pton(AF_INET, socket_config->ip, &servaddr.sin_addr) <= 0) {
        LOGE("inet_pton faild for %s \n", socket_config->ip);
        close(context->fd);
        return ERR_FAILD;
    }
    LOGI("ip: %s, port: %d \r\n", socket_config->ip, socket_config->port);

    if (connect(context->fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        LOGE("connect faild \n");
        close(context->fd);
        return ERR_FAILD;
    }

    if (socket_config->non_block_mode) {
        int flags = fcntl(context->fd, F_GETFL, 0);
        fcntl(context->fd, F_SETFL, flags | O_NONBLOCK);
    }

    LOGD("fd: %d \n", context->fd);
    return ERR_OK;
}

int HySocketRead(void *handle, char *buf, uint32_t len)
{
    if (!handle || !buf) {
        LOGE("the param is NULL \n");
        return -1;
    }
    socket_context_t *context = handle;

    int ret = recv(context->fd, buf, len, 0);
    if (ret <= 0) {
        if (0 == ret) {
            LOGE("recv data error, err: %d<%s> \n", errno, strerror(errno));
        } else if (ret < 0) {
            if (errno == EINTR) {
                ret = 0;
            }
        }
    }

    return ret;
}

int HySocketWrite(void *handle, char *buf, uint32_t len)
{
    if (!handle || !buf) {
        LOGE("the param is NULL \n");
        return -1;
    }
    socket_context_t *context = handle;

    int ret = send(context->fd, buf, len, MSG_NOSIGNAL);
    if (ret <= 0) {
        if (0 == ret) {
            LOGE("send data error, err: %d<%s> \n", errno, strerror(errno));
        } else if (ret < 0) {
            if (errno == EINTR) {
                ret = 0;
            }
        }
    }

    return ret;
}

void *HySocketCreate(HySocketConfig_t *socket_config)
{
    if (!socket_config) {
        LOGE("the param is NULL \n");
        return NULL;
    }

    socket_context_t *context = calloc(1, sizeof(*context));
    if (!context) {
        LOGE("calloc faild \n");
        return NULL;
    }

    if (ERR_OK != _socket_init(context, socket_config)) {
        LOGE("init socket faild \n");
        free(context);
        return NULL;
    }

    LOGI("hy_socket create successful \n");
    return context;
}

void HySocketDestroy(void *handle)
{
    if (!handle) {
        LOGE("the param is NULL \n");
        return ;
    }
    socket_context_t *context = handle;

    close(context->fd);
    free(context);
}

