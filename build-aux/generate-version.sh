#!/usr/bin/env bash

# set -x

relative_dir=..
m4_dir=${relative_dir}/m4

get_version_num_file()
{
    if git describe | sed s/-/_/g > version.num.tmp
    then
        mv version.num.tmp version.num
    else
        rm version.num.tmp
    fi
}

get_full_version_num()
{
    full_version=`tr -d '\n' < version.num`
    current=`cat version.num    | awk -F '_' '{print $1}' | awk -F '.' '{print $1}'`
    revision=`cat version.num | awk -F '_' '{print $1}' | awk -F '.' '{print $2}'`
    age=`cat version.num      | awk -F '_' '{print $1}' | awk -F '.' '{print $3}'`

    echo "full_version=$full_version"
    mkdir -p ${m4_dir}

    echo "m4_define([VERSION_NUM],          [$full_version])" >  ${m4_dir}/version.m4
    echo "m4_define([VERSION_CURRENT_NUM],  [$current])"        >> ${m4_dir}/version.m4
    echo "m4_define([VERSION_REVISION_NUM], [$revision])"     >> ${m4_dir}/version.m4
    echo "m4_define([VERSION_AGE_NUM],      [$age])"          >> ${m4_dir}/version.m4
}

echo "### get version num..."

get_version_num_file
get_full_version_num

