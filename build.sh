#!/usr/bin/env bash

# set -x

current_dir=`pwd`

echo -n "please input install absolute path: "
read install_path
echo "install path: ${install_path}"

echo -en "\nplease input build path: "
read build_path
echo "build path: ${build_path}"

echo -en "\nthe libhal install path: "
read libhal_install_path
config_str="--with-libhal=${libhal_install_path}"

echo -en "\nwhether to support libcjson[yes/no, default no]: "
read support_libcjson
if [[ ${support_libcjson} = yes ]]; then
    echo -en "\nlibcjson install path: "
    read libcjson_install_path
    config_str="${config_str} --enable-libcjson --with-libcjson=${libcjson_install_path}"
fi

echo "---------------------------------------------"
echo "detect system, initialize basic configuration"
echo "---------------------------------------------"
./autogen.sh

echo "-----------------------------------"
echo "run configure for generate Makefile"
echo "-----------------------------------"
mkdir -p ${build_path}
cd ${build_path}
${current_dir}/configure \
    --prefix=${install_path} \
    --program-prefix=${program_prefix} \
    ${config_str}

echo "----------------------"
echo " make and make install"
echo "----------------------"
make ; make install

