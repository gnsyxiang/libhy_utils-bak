/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    core_dump.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/11 2019 17:11
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/11 2019      create the file
 * 
 *     last modified: 06/11 2019 17:11
 */
#include <stdio.h>
#include <sys/resource.h>
#include <string.h>
#include <errno.h>

#include "core_dump.h"

#define CORE_SIZE   (1024 * 1024 * 500)


#define core_dump_log(fmt, ...) \
    printf("<%s:%d, result: %s> " fmt, \
           __func__, __LINE__, strerror(errno), ##__VA_ARGS__);

static void _get_rlimit_info(struct rlimit *rlmt, char *name)
{
    if (getrlimit(RLIMIT_CORE, rlmt) == -1) {
        core_dump_log("getrlimit faild \n");
        return ;
    }
    core_dump_log("%s: CORE dump current is:%d, max is:%d\n", \
                  name, (int)rlmt->rlim_cur, (int)rlmt->rlim_max);
}

void Core_OpenDump(void)
{
    struct rlimit rlmt;

    _get_rlimit_info(&rlmt, "before");

    rlmt.rlim_cur = (rlim_t)CORE_SIZE;
    rlmt.rlim_max  = (rlim_t)CORE_SIZE;

    if (setrlimit(RLIMIT_CORE, &rlmt) == -1) {
        core_dump_log("setrlimit faild \n");
        return ;
    }

    _get_rlimit_info(&rlmt, "after");
}

