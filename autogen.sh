#!/bin/sh

TOP_DIR=.
BUILD_AUX_DIR=${TOP_DIR}/build-aux

# generate version number
generate_version_num()
{
    cd ${BUILD_AUX_DIR}
    ./generate-version.sh
    cd -  >/dev/null 2>&1
}

# use autotools to configure
autotools_configure()
{
    autoreconf -fvi -Im4
}

generate_version_num

autotools_configure
