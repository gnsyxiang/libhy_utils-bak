/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    data_buf_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/11 2019 15:22
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/11 2019      create the file
 * 
 *     last modified: 05/11 2019 15:22
 */
#include <stdio.h>
#include <string.h>

#include "data_buf.h"

#define TEST_DATA_BUF_SIZE (8)

int main(int argc, const char *argv[])
{
    DataBufConfig_t databuf_config;
    databuf_config.ability = DATABUF_ABILITY_DISCARD_DATA;
    databuf_config.size    = TEST_DATA_BUF_SIZE;

    void *handle = DataBufInit(&databuf_config);

    int ret;
    char *buf = "123456789";
    int len = strlen(buf);
    ret = DataBufWrite(handle, buf, len);
    printf("ret: %d \n", ret);

    DataBufDump(handle);

// int DataBufRead(void *handle, void *buf, int len);

    DataBufFinal(handle);
    
    return 0;
}
