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
#include "hy_log4cplus_wrapper.h"

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

using namespace log4cplus;

typedef struct {
    char        *buf;
    uint32_t    buf_len;

    Logger      root;
} context_t;

static context_t _context;

void HyLog4cplusCreate(uint32_t buf_len, const char *config_file)
{
    if (!config_file) {
        printf("the config file is NULL\n");
        return ;
    }

    _context.buf_len = buf_len;
    _context.buf = (char *)calloc(1, _context.buf_len);
    if (!_context.buf) {
        printf("calloc faild \n");
        return ;
    }

    helpers::LogLog::getLogLog()->setInternalDebugging(false);
    PropertyConfigurator::doConfigure(config_file);

    _context.root = Logger::getRoot();
}

void HyLog4cplusDestroy(void)
{
    if (_context.buf) {
        free(_context.buf);
    }
}

void HyLog4cplusWrite(LogLevel level, const char *file,
        const char *func, uint32_t line, char *fmt, ...)
{
    if (_context.root.isEnabledFor(level)) {
        memset(_context.buf, '\0', _context.buf_len);

        // 加线程锁
        va_list args;
        va_start(args, fmt);
        vsnprintf(_context.buf, _context.buf_len, fmt, args);
        va_end(args);

        _context.root.forcedLog(level, _context.buf, file, line, func);
    }
}

