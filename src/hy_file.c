/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    file_wrapper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    21/11 2019 19:28
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        21/11 2019      create the file
 * 
 *     last modified: 21/11 2019 19:28
 */
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "hy_file.h"

uint32_t HyFileWrite(int fd, void *buf, uint32_t len)
{
    int ret;
    size_t nleft;
    const void *ptr;

    ptr   = buf;
    nleft = len;

    while (nleft > 0) {
        ret = write(fd, ptr, nleft);
        if (ret <= 0) {
            if (ret < 0 && errno == EINTR) {
                ret = 0;
            }
            else {
                return -1;
            }
        }

        nleft -= ret;
        ptr   += ret;
    }

    return len;
}

uint32_t HyFileRead(int fd, void *buf, uint32_t len)
{
    int ret;
    size_t nleft;
    size_t offset = 0;

    nleft = len;

    while (nleft > 0) {
        ret = read(fd, buf + offset, nleft);
        // printf("file_wrapper->read, len: %d \n", ret);
        if (ret < 0) {
            if (errno == EINTR) {
                ret = 0;
            } else {
                return -1;
            }
        } else if (ret == 0) {
            break;
        }

        nleft  -= ret;
        offset += ret;
    }

    return offset;
}

