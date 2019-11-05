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

#include "utils.h"
#include "data_buf.h"

#define TEST_DATA_BUF_SIZE (8)

void write_databuf(void *handle)
{
    int ret;
    char *buf = "1234567";
    int len = strlen(buf);
    ret = DataBufWrite(handle, buf, len);
    printf("ret: %d \n", ret);

    printf("----1----size: %d \n", DataBufGetSize(handle));
    DataBufDump(handle);
}

void read_databuf(void *handle)
{
    printf("\n\n-----size: %d \n", DataBufGetSize(handle));

    char read_buf[TEST_DATA_BUF_SIZE] = {0};
    int ret = DataBufRead(handle, read_buf, 3);

    DumpHexData(read_buf, ret);
    DataBufDump(handle);
}

void peek_read_databuf(void *handle)
{
    printf("\n\n-----size: %d \n", DataBufGetSize(handle));

    char read_buf[TEST_DATA_BUF_SIZE] = {0};
    int ret = DataBufPeekRead(handle, read_buf, 3);

    DumpHexData(read_buf, ret);
    DataBufDump(handle);
}

int main(int argc, const char *argv[])
{
    DataBufConfig_t databuf_config;
    databuf_config.m_ability = DATABUF_ABILITY_DISCARD_DATA;
    databuf_config.m_size    = TEST_DATA_BUF_SIZE;

    void *handle = DataBufInit(&databuf_config);

    write_databuf(handle);

    // peek_read_databuf(handle);
    // peek_read_databuf(handle);
    read_databuf(handle);
    read_databuf(handle);

    DataBufFinal(handle);
    
    return 0;
}
