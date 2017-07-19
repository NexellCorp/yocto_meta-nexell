#!/bin/bash

set -e

argc=$#
CURRENT_PATH=`dirname $0`
TOOLS_PATH=`readlink -ev $CURRENT_PATH`

RESULT_DIR=
RESULT_PATH=
IMAGE_TYPE=$2

MACHINE_NAME=$1
BOARD_SOCNAME=
BOARD_NAME=
BOARD_PREFIX=
BOARD_POSTFIX=

META_NEXELL_PATH=`readlink -ev ${TOOLS_PATH}/..`
BUILD_PATH=
TMP_DEPLOY_PATH=

function split_args()
{
    BOARD_SOCNAME=${MACHINE_NAME%-*-*}
    BOARD_NAME=${MACHINE_NAME#*-}
    BOARD_PREFIX=${BOARD_NAME%-*}
    BOARD_POSTFIX=${BOARD_NAME#*-}

    RESULT_DIR="SDK-result-${BOARD_SOCNAME}-${IMAGE_TYPE}"
}

function path_setup()
{
    BUILD_PATH=`readlink -ev ${META_NEXELL_PATH}/../../build/SDK-build-${BOARD_SOCNAME}-${IMAGE_TYPE}`
    RESULT_PATH=${BUILD_PATH}/../../out/${RESULT_DIR}
    TMP_DEPLOY_PATH=${BUILD_PATH}/tmp/deploy/sdk/
}

function cleanup_dirs()
{
    if [ ! -d ${RESULT_PATH} ];then
	mkdir -p ${RESULT_PATH}
        chmod 777 ${RESULT_PATH}
    fi
    RESULT_PATH=`readlink -ev ${RESULT_PATH}`
}

function copy_sdk_images()
{
    echo -e "\033[40;33m  >>>>   copy_sdk_images        \033[0m"
    cp ${TMP_DEPLOY_PATH}/* ${RESULT_PATH}

    echo -e "\n\033[0;34m ------------------------------------------------------------------------------------------ \033[0m"
    echo -e "\033[0;33m # SDK IMAGE READY #                                                                          \033[0m"
    echo -e "\033[0;36m    Run : \$ ${RESULT_PATH}/poky-xxxx-toolchain-2.x.x.sh                                      \033[0m"
    echo -e "\033[0;34m ------------------------------------------------------------------------------------------ \033[0m\n"
}

function post_process()
{
    if [ -f secure.cfg ]; then
	cp secure.cfg ${RESULT_PATH}
    fi

    touch ${RESULT_PATH}/YOCTO.${RESULT_DIR}.INFO.DoNotChange
}

split_args
path_setup
cleanup_dirs
copy_sdk_images
post_process
