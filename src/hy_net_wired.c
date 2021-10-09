/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_net_wired.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    09/10 2021 10:28
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        09/10 2021      create the file
 * 
 *     last modified: 09/10 2021 10:28
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <linux/types.h>

#include "hy_net_wired.h"

#include "hy_assert.h"
#include "hy_thread.h"
#include "hy_fifo.h"
#include "hy_mem.h"
#include "hy_type.h"
#include "hy_string.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

typedef struct {
    hy_s32_t led;
    hy_s32_t mode;
} _led_mode_t;

struct mii_data {
    hy_u16_t phy_id;
    hy_u16_t reg_num;
    hy_u16_t val_in;
    hy_u16_t val_out;
};

typedef struct {
    HyNetWiredSaveConfig_t save_config;

    void *led_thread_handle;
    void *led_blink_thread_handle;
    void *fifo_handle;

    hy_s32_t exit_flag;
} _net_wired_context_t;

static _net_wired_context_t *context = NULL;

static hy_s32_t _led_blink_loop_cb(void *args)
{
    while (!context->exit_flag) {
        sleep(1);
    }
    return -1;
}

#define _MII_SET(fd, ifr, reg_val)                                      \
    do {                                                                \
        struct mii_data *mii;                                           \
                                                                        \
        mii = (struct mii_data *)&(ifr).ifr_data;                       \
        mii->reg_num    = (reg_val).reg;                                \
        mii->val_in     = (reg_val).val;                                \
                                                                        \
        if (ioctl(fd, SIOCSMIIREG, &ifr) < 0) {                         \
            LOGE("ioctl failed SIOCSMIIREG on %s \n", ifr.ifr_name);    \
        }                                                               \
    } while (0);

static int32_t _led_set(_led_mode_t *led_mode)
{
    int32_t skfd = -1;
    struct ifreq ifr;
    HyNetWiredLed_t *led = context->save_config.led[led_mode->led];
    HyNetWiredRegVal_t *reg_val = led[led_mode->mode].reg_val;

    if ((skfd = socket(AF_INET, SOCK_DGRAM,0)) < 0) {
        perror("socket");
        return -1;
    }

    strncpy(ifr.ifr_name, context->save_config.dev_name, IFNAMSIZ);
    if (ioctl(skfd, SIOCGMIIPHY, &ifr) < 0) {
        if (errno != ENODEV) {
            LOGE("ioctl failed SIOCGMIIPHY on '%s' \n",
                    context->save_config.dev_name);
            return -1;
        }
    }

    _MII_SET(skfd, ifr, reg_val[0]);
    _MII_SET(skfd, ifr, reg_val[1]);

    close(skfd);

    return 0;
}

void HyNetWiredSetLed(hy_s32_t led, hy_s32_t mode)
{
    _led_mode_t led_mode;

    led_mode.led = led;
    led_mode.mode = mode;

    HyFifoPut(context->fifo_handle, &led_mode, sizeof(led_mode));
}

static hy_s32_t _led_loop_cb(void *args)
{
    size_t val = 0;
    _led_mode_t led_mode;

    while (!context->exit_flag) {
        while (0 == val) {
            HyFifoGetInfo(context->fifo_handle, HY_FIFO_INFO_USED_LEN, &val);
            usleep(100 * 1000);
        }

        HyFifoGet(context->fifo_handle, &led_mode, sizeof(led_mode));

        if (led_mode.mode == HY_NET_WIRED_LED_MODE_OFF
                || led_mode.mode == HY_NET_WIRED_LED_MODE_ON) {
            _led_set(&led_mode);
        } else {

        }
    }

    return -1;
}

void HyNetWiredDestroy(void **handle)
{
    context->exit_flag = 1;

    HyThreadDestroy(&context->led_thread_handle);
    HyThreadDestroy(&context->led_blink_thread_handle);

    HyFifoDestroy(&context->fifo_handle);

    HY_FREE_PP(&context);

    LOGI("net wired destroy successful \n");
}

void *HyNetWiredCreate(HyNetWiredConfig_t *config)
{
    HY_ASSERT_VAL_RET_VAL(!config, NULL);

    do {
        context = HY_MALLOC_BREAK(_net_wired_context_t *, sizeof(*context));
        HY_MEMCPY(&context->save_config, &config->save_config);

        HyFifoConfig_t fifo_config;
        fifo_config.save_config.size = sizeof(_led_mode_t) * 6;
        context->fifo_handle = HyFifoCreate(&fifo_config);
        if (!context->fifo_handle) {
            LOGE("HyFifoCreate failed \n");
            break;
        }

        HyThreadConfig_t led_config;
        led_config.save_config.thread_loop_cb    = _led_loop_cb;
        led_config.save_config.args              = context;
        HY_MEMCPY(&led_config.save_config.name, "net_led");
        context->led_thread_handle = HyThreadCreate(&led_config);
        if (!context->led_thread_handle) {
            LOGE("HyThreadCreate failed \n");
            break;
        }

        HyThreadConfig_t led_blink_config;
        led_blink_config.save_config.thread_loop_cb     = _led_blink_loop_cb;
        led_blink_config.save_config.args               = context;
        HY_MEMCPY(&led_blink_config.save_config.name, "net_led_blink");
        context->led_blink_thread_handle = HyThreadCreate(&led_blink_config);
        if (!context->led_blink_thread_handle) {
            LOGE("HyThreadCreate failed \n");
            break;
        }

        LOGI("net wired create successful \n");
        return context;
    } while (0);

    HyNetWiredDestroy(NULL);
    return NULL;
}

