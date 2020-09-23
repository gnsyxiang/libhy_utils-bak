/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_fifo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    23/09 2020 16:02
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        23/09 2020      create the file
 * 
 *     last modified: 23/09 2020 16:02
 */
#include <stdio.h>
#include <string.h>

#include "hy_fifo.h"

typedef struct {
    char        *buf;
    uint32_t    len;

    uint32_t    cnt;
    uint32_t    head;
    uint32_t    tail;
} fifo_context_t;
#define FIFO_CONTEXT_T_LEN  (sizeof(fifo_context_t))

static fifo_context_t fifo_context;

void HyFifoCreate(char *buf, uint32_t len)
{
    memset(&fifo_context, '\0', FIFO_CONTEXT_T_LEN);

    fifo_context.buf = buf;
    fifo_context.len = len;
}

void HyFifoDestroy(void)
{
    memset(&fifo_context, '\0', FIFO_CONTEXT_T_LEN);
}

void HyFifoDump(void)
{
}






