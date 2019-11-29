#!/bin/bash

set -e

argc=$#
PARENT_DIR="${PWD%/*}"
CURRENT_PATH=`dirname $0`
TOOLS_PATH=`readlink -ev $CURRENT_PATH`
RESULT_DIR=`readlink -ev $CURRENT_PATH/..`
RESULT_INFO=`readlink -ev $CURRENT_PATH/../YOCTO.*.INFO.*`

BUILD_NAME=
MACHINE_NAME=
BOARD_SOCNAME=
BOARD_NAME=
BOARD_PREFIX=
BOARD_POSTFIX=
TEMP_ARRAY=

function get_board_prefix()
{
    IFS='/' array=($RESULT_INFO)
    local temp="${array[-1]}"

    IFS='.' array=($temp)
    local RESULT_NAME="${array[1]}"

    BUILD_NAME=${RESULT_NAME#*-}
    OLD_IFS=$IFS
    IFS=-
    TEMP_ARRAY=($BUILD_NAME)
    IFS=$OLD_IFS

#    MACHINE_NAME=${BUILD_NAME%-*}
    MACHINE_NAME=${TEMP_ARRAY[0]}-${TEMP_ARRAY[1]}-${TEMP_ARRAY[2]}
    BOARD_SOCNAME=${MACHINE_NAME%-*-*}
    BOARD_NAME=${MACHINE_NAME#*-}
    BOARD_PREFIX=${BOARD_NAME%-*}
    BOARD_POSTFIX=${BOARD_NAME#*-}

    IFS=''
}

function run_by_usb()
{
    if [ ${BOARD_SOCNAME} == "s5p6818" ]; then
        if [ "${BOARD_NAME}" == "artik710-raptor" ]; then
            sudo ${TOOLS_PATH}/usb-downloader -t slsiap \
                 -b ${RESULT_DIR}/bl1-raptor.bin \
                 -a 0xFFFF0000 -j 0xFFFF0000
            sleep 1
            sudo ${TOOLS_PATH}/usb-downloader -t slsiap \
                 -f ${RESULT_DIR}/fip-loader-usb.img -m
        else
            sudo ${TOOLS_PATH}/usb-downloader -t slsiap \
                 -b ${RESULT_DIR}/bl1-${BOARD_PREFIX}.bin \
                 -a 0xFFFF0000 -j 0xFFFF0000
            sleep 1
            sudo ${TOOLS_PATH}/usb-downloader -t slsiap \
                 -f ${RESULT_DIR}/fip-loader-usb.img -m
        fi
    else
        if [ ${BOARD_PREFIX} == "avn" ]; then
            sudo ${TOOLS_PATH}/usb-downloader -t slsiap \
                 -b ${RESULT_DIR}/bl1-${BOARD_PREFIX}.bin \
                 -a 0xFFFF0000 -j 0xFFFF0000
            sleep 1
            sudo ${TOOLS_PATH}/usb-downloader -t slsiap \
                 -f ${RESULT_DIR}/fip-loader-usb.img -m
        elif [ ${BOARD_PREFIX} == "svt" ]; then
            sudo ${TOOLS_PATH}/usb-downloader -t slsiap \
                 -b ${RESULT_DIR}/bl1-${BOARD_PREFIX}.bin \
                 -a 0xFFFF0000 -j 0xFFFF0000
            sleep 1
            sudo ${TOOLS_PATH}/usb-downloader -t slsiap \
                 -f ${RESULT_DIR}/fip-loader-usb.img -m
        else
            echo ${TOOLS_PATH}
            sudo ${TOOLS_PATH}/usb-downloader -t nxp4330 \
                 -b ${RESULT_DIR}/bl1-${BOARD_PREFIX}.bin \
                 -a 0xFFFF0000 -j 0xFFFF0000
            sleep 3
            sudo ${TOOLS_PATH}/usb-downloader -t nxp4330 \
                 -f ${RESULT_DIR}/fip-loader-usb.img -m
        fi
    fi
}

get_board_prefix
run_by_usb
