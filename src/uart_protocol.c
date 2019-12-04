/**
 *
 * Release under GPL-3.0.
 *
 * @file    uart_protocol.c
 * @brief
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/11 2019 10:07
 * @version v0.0.1
 *
 * @since    note
 * @note     note
 *
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/11 2019      create the file
 *
 *     last modified: 26/11 2019 10:07
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <semaphore.h>

#define LIBUTILS_INC_UART_PROTOCOL_GB
#include "uart_protocol.h"
#undef LIBUTILS_INC_UART_PROTOCOL_GB
#include "uart_protocol_inside.h"
#include "uart_protocol_v1.h"

#include "uart_wrapper.h"
#include "thread_wrapper.h"
#include "list.h"
#include "file_wrapper.h"

#define uart_protocol_log(fmt, ...) \
    printf("<%s:%d, result: %s> " fmt, \
           __func__, __LINE__, strerror(errno), ##__VA_ARGS__);

static void _read_frame_loop(void *args);
static void _write_frame_loop(void *args);

typedef enum {
    UART_PROTOCOL_V1,
    UART_PROTOCOL_V2,

    UART_PROTOCOL_MAX,
} uart_protocol_version_t;

typedef int (*uart_protocol_init_cb_t)(void);
typedef int (*uart_protocol_final_cb_t)(void);
typedef int (*uart_protocol_encode_cb_t)(unsigned char **frame, buf_t *buf, UartProtocolCmd_t *cmd);
typedef int (*uart_protocol_decode_cb_t)(buf_t *buf, frame_cnt_t *frame_cnt);
typedef int (*uart_protocol_sync_state_cb_t)(char *frame, UartProtocolState_t * const state);
typedef int (*uart_protocol_set_state_cb_t)(UartProtocolCmd_t *cmd, buf_t *buf);

typedef struct {
    uart_protocol_init_cb_t         init_cb;
    uart_protocol_final_cb_t        final_cb;
    uart_protocol_encode_cb_t       encode_cb;
    uart_protocol_decode_cb_t       decode_cb;
    uart_protocol_sync_state_cb_t   sync_state_cb;
    uart_protocol_set_state_cb_t    set_state_cb;
} uart_protocol_cb_t;

typedef struct {
    struct list_head    list;
    pthread_mutex_t     mutex;
    sem_t               sem_write;
} list_wrapper_t;

typedef struct {
    sem_t   sem_read_thread_exit;
    sem_t   sem_write_thread_exit;
} creat_threads_t;

/**
 * @brief 
 */
typedef struct {
    UartProtocolConfig_t    config;
    int                     fd;
    int                     is_running;
    int                     version;
    int                     lock_version;

    UartProtocolState_t     state;
    list_wrapper_t          list_wrapper;
    creat_threads_t         creat_threads;
    uart_protocol_cb_t      protocol_cb;
} uart_protocol_state_t;

typedef struct {
    unsigned char       *frame;
    int                 len;
    struct list_head    list;
} frame_list_t;

static frame_list_t *_frame_list_init(uart_protocol_state_t *uart_protocol_state, buf_t *buf, UartProtocolCmd_t *cmd)
{
    frame_list_t *frame_list = calloc(1, DATA_TYPE_LEN(frame_list_t));
    if (!frame_list) {
        uart_protocol_log("calloc is faild \n");
        return NULL;
    }

    uart_protocol_cb_t *protocol_cb = &uart_protocol_state->protocol_cb;
    frame_list->len = protocol_cb->encode_cb(&frame_list->frame, buf, cmd);
    if (-1 == frame_list->len) {
        uart_protocol_log("calloc is faild \n");
        free(frame_list);
        return NULL;
    }

    return frame_list;
}

static inline void _frame_list_final(frame_list_t *frame_list)
{
    if (frame_list) {
        if (frame_list->frame) {
            free(frame_list->frame);
        }
        free(frame_list);
    }
}

static inline void _list_wrapper_init(list_wrapper_t *list_wrapper)
{
    INIT_LIST_HEAD(&list_wrapper->list);
    pthread_mutex_init(&list_wrapper->mutex, NULL);
    sem_init(&list_wrapper->sem_write, 0, 0);
}

static inline void _list_wrapper_final(list_wrapper_t *list_wrapper)
{
    sem_destroy(&list_wrapper->sem_write);
}

static inline void _creat_thread_init(creat_threads_t *creat_threads, void *args)
{
    sem_init(&creat_threads->sem_read_thread_exit, 0, 0);
    sem_init(&creat_threads->sem_write_thread_exit, 0, 0);

    ThreadParam_t thread_param;
    memset(&thread_param, '\0', DATA_TYPE_LEN(thread_param));
    thread_param.thread_loop = _write_frame_loop;
    thread_param.name        = "write";
    thread_param.args        = args;
    Thread_CreateDetachedThread(&thread_param);

    thread_param.thread_loop = _read_frame_loop;
    thread_param.name        = "read";
    Thread_CreateDetachedThread(&thread_param);
}

static inline void _creat_thread_final(creat_threads_t *creat_threads)
{
    sem_wait(&creat_threads->sem_write_thread_exit);
    sem_wait(&creat_threads->sem_read_thread_exit);

    sem_destroy(&creat_threads->sem_read_thread_exit);
    sem_destroy(&creat_threads->sem_write_thread_exit);
}

static int _write_frame(void *handle, UartProtocolCmd_t *cmd)
{
    buf_t buf;
    Memset(buf);

    uart_protocol_state_t *uart_protocol_state = (uart_protocol_state_t *)handle;

    if (uart_protocol_state->version == UART_PROTOCOL_MAX) {
        uart_protocol_log("the version is UART_PROTOCOL_MAX \n");
        return -1;
    }

    uart_protocol_state->protocol_cb.set_state_cb(cmd, &buf);

    frame_list_t *frame_list = _frame_list_init(uart_protocol_state, &buf, cmd);
    if (!frame_list) {
        uart_protocol_log("_frame_list_init faild \n");
        return -1;
    }

    list_wrapper_t *list_wrapper = &uart_protocol_state->list_wrapper;
    pthread_mutex_lock(&list_wrapper->mutex);
    list_add_tail(&frame_list->list, &list_wrapper->list);
    pthread_mutex_unlock(&list_wrapper->mutex);

    sem_post(&list_wrapper->sem_write);

    return 0;
}

static void _lock_version_loop(void *args)
{
    UartProtocolCmd_t cmd;
    uart_protocol_version_t version = UART_PROTOCOL_V1;
    uart_protocol_state_t *uart_protocol_state = args;
    uart_protocol_cb_t protocol_cb[UART_PROTOCOL_MAX] = {
        {UartProtocolV1Init, UartProtocolV1Final, UartProtocolV1Encode, UartProtocolV1Decode, UartProtocolV1SyncState, UartProtocolV1SetState},
    };

    cmd.cmd = UART_PROTOCOL_CMD_QUERY;
    cmd.val = UART_PROTOCOL_VAL_NONE;

    while (!uart_protocol_state->lock_version) {
        if (version == UART_PROTOCOL_MAX) {
            version = UART_PROTOCOL_V1;
        }
        uart_protocol_state->version = version++;
        uart_protocol_state->protocol_cb = protocol_cb[uart_protocol_state->version];

        if (_write_frame(args, &cmd) < 0) {
            uart_protocol_log("_write_frame faild \n");
        }

        //FIXME
        uart_protocol_state->lock_version = 1;
    }
}

static void _lock_version(void *args)
{
    ThreadParam_t thread_param;
    memset(&thread_param, '\0', DATA_TYPE_LEN(thread_param));
    thread_param.thread_loop = _lock_version_loop;
    thread_param.name        = "lock_version";
    thread_param.args        = args;
    Thread_CreateDetachedThread(&thread_param);
}

static void _write_frame_loop(void *args)
{
    uart_protocol_log("_write_frame_loop start \n");

    frame_list_t *pos, *n;
    uart_protocol_state_t *uart_protocol_state = args;
    list_wrapper_t *list_wrapper = &uart_protocol_state->list_wrapper;

    while (uart_protocol_state->is_running) {
        sem_wait(&list_wrapper->sem_write);

        pthread_mutex_lock(&list_wrapper->mutex);
        list_for_each_entry_safe(pos, n, &list_wrapper->list, list) {
            if (-1 == UartWrite(uart_protocol_state->fd, pos->frame, pos->len)) {
                uart_protocol_log("uart write internel faild \n");
                continue;
            }
            list_del(&pos->list);
            _frame_list_final(pos);
            break;
        }
        pthread_mutex_unlock(&list_wrapper->mutex);
    }
    sem_post(&uart_protocol_state->creat_threads.sem_write_thread_exit);
}

static void _read_frame_loop(void *args)
{
    uart_protocol_log("_read_frame_loop start \n");

    buf_t buf;
    frame_cnt_t frame_cnt;
    size_t cnt = 0;
    uart_protocol_state_t *uart_protocol_state = args;
    uart_protocol_cb_t    *protocol_cb = &uart_protocol_state->protocol_cb;

    while (uart_protocol_state->is_running) {
        buf.len = UartRead(uart_protocol_state->fd, buf.buf, UART_READ_VMIN_LEN);
        if (buf.len > 0 && (cnt = protocol_cb->decode_cb(&buf, &frame_cnt)) > 0) {
            for (size_t i = 0; i < cnt; i++) {
                // UartProtocolDumpHex("--------------->>>", frame_cnt.frame[i], frame_cnt.len[i]);
                protocol_cb->sync_state_cb(frame_cnt.frame[i], &uart_protocol_state->state);
                uart_protocol_state->config.read_cb(&uart_protocol_state->state);
                free(frame_cnt.frame[i]);
                frame_cnt.frame[i] = NULL;
            }

        }
    }

    sem_post(&uart_protocol_state->creat_threads.sem_read_thread_exit);
}

void *UartProtocolInit(UartProtocolConfig_t *config)
{
    if (!config) {
        uart_protocol_log("the config is NULL \n");
        return NULL;
    }

    uart_protocol_state_t *uart_protocol_state = calloc(1, DATA_TYPE_LEN(uart_protocol_state_t));
    if (!uart_protocol_state) {
        uart_protocol_log("calloc is faild \n");
        return NULL;
    }

    uart_protocol_state->is_running = 1;

    UartConfig_t uart_config;
    uart_config.num          = UART_NUM_0;
    uart_config.speed        = UART_SPEED_115200;
    uart_config.flow_control = FLOW_CONTROL_NONE;
    uart_config.data_bit     = DATA_BIT_8;
    uart_config.parity_type  = PARITY_NONE;
    uart_config.stop_bit     = STOP_BIT_1;

    uart_protocol_state->fd = UartInit(&uart_config);
    if (uart_protocol_state->fd < 0) {
        uart_protocol_log("calloc is faild \n");
        free(uart_protocol_state);
        return NULL;
    }

    // 结构体里面的指针不会有两次释放的操作，所以没有风险
    uart_protocol_state->config = *config;

    _creat_thread_init(&uart_protocol_state->creat_threads, uart_protocol_state);
    _list_wrapper_init(&uart_protocol_state->list_wrapper);

    uart_protocol_state->version = UART_PROTOCOL_MAX;
    _lock_version(uart_protocol_state);

    uart_protocol_log("uart protocol init successful \n");

    return (void *)uart_protocol_state;
}

int UartProtocolFinal(void *handle)
{
    if (handle) {
        uart_protocol_state_t *uart_protocol_state = handle;

        uart_protocol_state->is_running = 0;
        _creat_thread_final(&uart_protocol_state->creat_threads);
        _list_wrapper_final(&uart_protocol_state->list_wrapper);

        if (uart_protocol_state->fd > 0) {
            UartFinal(uart_protocol_state->fd);
            uart_protocol_log("close uart fd \n");
        }
        free(uart_protocol_state);
    }
    return 0;
}


int UartProtocolWriteFrame(void *handle, UartProtocolCmd_t *cmd)
{
    if (!handle) {
        uart_protocol_log("the handle is NULL \n");
        return -1;
    }

    if (!cmd) {
        uart_protocol_log("the cmd is NULL \n");
        return -1;
    }

    return _write_frame(handle, cmd);
}

void UartProtocolDumpHex(char *sign, char *buf, size_t len)
{
    printf("%s, len[%ld]: ", sign, len);
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", (unsigned char)buf[i]);
    }
    printf("\n");
}

