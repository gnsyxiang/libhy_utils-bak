/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_log4cplus_wrapper.cpp
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    01/06 2021 14:51
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        01/06 2021      create the file
 * 
 *     last modified: 01/06 2021 14:51
 */
#include "hy_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <log4cplus/config.hxx>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/socket.h>
#include <log4cplus/loggingmacros.h>

#include "hy_type.h"

using namespace log4cplus;

typedef struct {
    char        *buf;
    uint32_t    buf_len;

    Logger      root;
} context_t;

static context_t *_context = nullptr;

void HyLogCreate(int32_t level, uint32_t buf_len, const char *config_file)
{
    if (!config_file) {
        printf("the config file is NULL\n");
        return ;
    }

    _context = new context_t();
    if (!_context) {
        printf("new context_t faild \n");
        return;
    }

    _context->buf_len = buf_len;
    _context->buf = new char[buf_len];
    if (!_context->buf) {
        printf("new char faild \n");
        return ;
    }

    helpers::LogLog::getLogLog()->setInternalDebugging(false);
    PropertyConfigurator::doConfigure(config_file);

    _context->root = Logger::getRoot();
}

void HyLogDestroy(void)
{
    if (_context) {
        if (_context->buf) {
            delete []_context->buf;
        }
        delete _context;
    }
}

void HyLogWrite(LogLevel level, const char *file,
        const char *func, uint32_t line, char *fmt, ...)
{
    if (_context && _context->root.isEnabledFor(level)) {
        memset(_context->buf, '\0', _context->buf_len);

        // 加线程锁
        va_list args;
        va_start(args, fmt);
        vsnprintf(_context->buf, _context->buf_len, fmt, args);
        va_end(args);

        _context->root.forcedLog(level, _context->buf, file, line, func);
    }
}

void PrintHex(const char *name, hy_uint16_t line,
        const char *buf, hy_int32_t len, hy_int8_t flag)
{
    if (len <= 0) {
        return;
    }

    hy_uint8_t cnt = 0;
    printf("[%s %d]len: %d \r\n", name, line, len);
    for (int i = 0; i < len; i++) {
        if (flag == 1) {
            if (buf[i] == 0x0d || buf[i] == 0x0a || buf[i] < 32 || buf[i] >= 127) {
                printf("%02x[ ]  ", (hy_uint8_t)buf[i]);
            } else {
                printf("%02x[%c]  ", (hy_uint8_t)buf[i], (hy_uint8_t)buf[i]);
            }
        } else {
            printf("%02x ", (hy_uint8_t)buf[i]);
        }
        cnt++;
        if (cnt == 16) {
            cnt = 0;
            printf("\r\n");
        }
    }
    printf("\r\n");
}
