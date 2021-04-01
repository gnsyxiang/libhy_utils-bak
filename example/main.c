/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    main.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    18/03 2021 20:22
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        18/03 2021      create the file
 * 
 *     last modified: 18/03 2021 20:22
 */
#include <stdio.h>

#include "autotools-demo/module_a.h"
#include "autotools-demo/sub_include/module_b.h"

int main(int argc, char const* argv[])
{
    printf("hello world \n");

    HyModuleAInit();
    HyModuleBInit();

    return 0;
}
