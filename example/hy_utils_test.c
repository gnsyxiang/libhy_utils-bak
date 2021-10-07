/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_utils_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    07/10 2021 13:54
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        07/10 2021      create the file
 * 
 *     last modified: 07/10 2021 13:54
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hy_module.h"
#include "hy_mem.h"
#include "hy_type.h"
#include "hy_utils.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

typedef struct {
    void *log_handle;
} _main_context_t;

static void _module_destroy(_main_context_t **context_pp)
{
    _main_context_t *context = *context_pp;

    // note: 增加或删除要同步到module_create_t中
    module_destroy_t module[] = {
        {"log",     &context->log_handle,   HyLogDestroy},
    };

    RUN_DESTROY(module);

    HY_FREE_PP(context_pp);
}

static _main_context_t *_module_create(void)
{
    _main_context_t *context = HY_MALLOC_RET_VAL(_main_context_t *, sizeof(*context), NULL);

    HyLogConfig_t log_config;
    log_config.save_config.buf_len      = 512;
    log_config.save_config.level        = HY_LOG_LEVEL_TRACE;
    log_config.save_config.color_output = HY_FLAG_ENABLE;

    // note: 增加或删除要同步到module_destroy_t中
    module_create_t module[] = {
        {"log",  &context->log_handle,   &log_config,    (create_t)HyLogCreate,    HyLogDestroy},
    };

    RUN_CREATE(module);

    return context;
}

int main(int argc, char *argv[])
{
    _main_context_t *context = _module_create();
    if (!context) {
        LOGE("_module_create faild \n");
        return -1;
    }

    LOGI("version: %s, data: %s, time: %s \n", "0.1.0", __DATE__, __TIME__);

    {
        uint32_t ip_num = 0;
        char *ip_str = "192.168.1.110";

        HyIpStr2Int(ip_str, &ip_num);
        LOGE("ip_num: %u \n", ip_num);
    }

    {
        uint32_t ip_num = 3232235886;
        char ip_str[IP_STR_DOT_LEN] = {0};

        HyIpInt2Str(ip_num, ip_str, sizeof(ip_str));
        LOGE("ip_str: %s \n", ip_str);
    }

    {
        char addr[] = {0x10, 0x11, 0x12};
        char buf[BUF_LEN] = {0};

        HyHex2Int2Str(addr, HyUtilsArrayCnt(addr), buf, sizeof(buf));
        LOGE("buf: %s \n", buf);
    }

    {
        char *buf = "016017018";
        char addr[3];

        HyStr2Int2Hex(buf, strlen(buf), addr, 3);
        for (int i = 0; i < 3; ++i) {
            LOGE("%02x \n", addr[i]);
        }
    }

    {
        uint32_t dec = 0;
        char *buf = "10101010";
        dec = HyBitStr2Dec(buf, strlen(buf));
        LOGE("dec: %d, %02x \n", dec, dec);
    }

    {
        uint32_t dec = 0xaa;
        char buf[BUF_LEN] = {0};

        HyDec2BitStr(dec, 8, buf, sizeof(buf));
        LOGE("buf: %s \n", buf);
    }

    _module_destroy(&context);

    return 0;
}

