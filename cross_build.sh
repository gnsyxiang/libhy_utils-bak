#!/usr/bin/env bash

# set -x

target=libhy_utils

data_disk_path=/opt/data
top_dir_path=`pwd`

vender=hisi
host=arm-himix200-linux
gcc_version=arm-himix200-linux
gcc_prefix=arm-himix200-linux

# gcc
cross_gcc_path=${data_disk_path}/opt/toolchains/${vender}/${gcc_version}/bin/${gcc_prefix}-

# cross lib on pc
lib_3rd_path=${data_disk_path}/install/${vender}/${gcc_version}

# target
target_path=${top_dir_path}
# prefix_path=${data_disk_path}/nfs/meian/app
prefix_path=${lib_3rd_path}

cd ${target_path} && ./autogen.sh && cd -

mkdir -p _build
cd _build

${target_path}/configure        \
    CC=${cross_gcc_path}gcc     \
    CXX=${cross_gcc_path}g++    \
    CPPFLAGS=""                 \
    CFLAGS=""                   \
    CXXFLAGS=""                 \
    LDFLAGS=""                  \
    LIBS=""                     \
    PKG_CONFIG_PATH=""          \
    --prefix=${prefix_path}     \
    --build=                    \
    --host=${host}              \
    --target=${host}

make -j4; make install

