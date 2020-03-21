#!/usr/bin/env bash

if [ $# != 2 ] ; then
    echo "usage: $0 absolute_install_path 3rd_lib_path"
    echo ""
    echo " e.g.: $0 "'`pwd`/install `pwd`/3rd_lib'
    echo ""
    exit 1;
fi

echo "---------------------------------------------"
echo "detect system, initialize basic configuration"
echo "---------------------------------------------"
./autogen.sh

echo "-----------------------------------"
echo "run configure for generate Makefile"
echo "-----------------------------------"
./configure \
    --with-libhal=${2}/libhal \
    --prefix=${1} \

echo "----------------------"
echo " make and make install"
echo "----------------------"
make ; make install
