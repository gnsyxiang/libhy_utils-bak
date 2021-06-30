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

${target_path}/configure                            \
    CC=${cross_gcc_path}gcc                         \
    CXX=${cross_gcc_path}g++                        \
    CPPFLAGS=""                                     \
    CFLAGS="-I${lib_3rd_path}/include"              \
    CXXFLAGS="-I${lib_3rd_path}/include"            \
    LDFLAGS="-L${lib_3rd_path}/lib"                 \
    LIBS=""                                         \
    PKG_CONFIG_PATH="${lib_3rd_path}/lib/pkgconfig" \
    --prefix=${prefix_path}                         \
    --build=                                        \
    --host=${host}                                  \
    --target=${host}

thread_jobs=`getconf _NPROCESSORS_ONLN 2>/dev/null || echo 1`

make -j${thread_jobs}; make install

