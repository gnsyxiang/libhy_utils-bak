/**
 * 
 * Release under GPL-3.0.
 * 
 * @file    hy_list_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/04 2020 16:12
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/04 2020      create the file
 * 
 *     last modified: 08/04 2020 16:12
 */
#include <stdio.h>

#include "hy_list.h"

#define MSG_CNT     (5)

static struct list_head g_list_head;

struct msg {
    int32_t cnt;
    struct list_head list;
};

int32_t main(int32_t argc, const hal_char_t *argv[])
{
    INIT_LIST_HEAD(&g_list_head);

    struct msg test[MSG_CNT];
    for (int32_t i = 0; i < MSG_CNT; i++) {
        test[i].cnt = i;
        // INIT_LIST_HEAD(&test[i].list);
        list_add_tail(&test[i].list, &g_list_head);
    }

    struct msg *pos, *n;
    list_for_each_entry(pos, &g_list_head, list) {
        if (pos) {
            printf("cnt: %d \n", pos->cnt);
        }
    }

    list_for_each_entry_safe(pos, n, &g_list_head, list) {
        printf("cnt: %d \n", pos->cnt);
        list_del(&pos->list);
    }

    return 0;
}

