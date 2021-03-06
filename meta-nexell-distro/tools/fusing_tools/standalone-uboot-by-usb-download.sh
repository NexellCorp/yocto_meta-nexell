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

function get_board_prefix()
{
    IFS='/' array=($RESULT_INFO)
    local temp="${array[-1]}"

    IFS='.' array=($temp)
    local RESULT_NAME="${array[1]}"

    BUILD_NAME=${RESULT_NAME#*-}
    MACHINE_NAME=${BUILD_NAME%-*}
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
        elif [ "${BOARD_NAME}" == "avn-ref" ]; then
            sudo ${TOOLS_PATH}/usb-downloader -t slsiap \
                 -b ${RESULT_DIR}/bl1-avn.bin \
                 -a 0xFFFF0000 -j 0xFFFF0000
            sleep 1
            sudo ${TOOLS_PATH}/usb-downloader -t slsiap \
                 -f ${RESULT_DIR}/fip-loader-usb.img -m
        elif [ "${BOARD_NAME}" == "bitminer-ref" ]; then
            sudo ${TOOLS_PATH}/usb-downloader -t slsiap \
                 -b ${RESULT_DIR}/bl1-bitminer.bin \
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
                 -f ${RESULT_DIR}/fip-nonsecure-usb.bin -a 0x63c00000 -j 0x63c00000
        elif [ ${BOARD_PREFIX} == "navi" -o ${BOARD_PREFIX} == "daudio" ]; then
            echo ${TOOLS_PATH}
            sudo ${TOOLS_PATH}/usb-downloader -t nxp4330 \
                 -b ${RESULT_DIR}/bl1-${BOARD_PREFIX}.bin \
                 -a 0xFFFF0000 -j 0xFFFF0000
            sleep 1
            if [ ${BOARD_POSTFIX} == "covi" ]; then
                sudo ${TOOLS_PATH}/usb-downloader -t nxp4330 \
                     -f ${RESULT_DIR}/fip-nonsecure-usb.bin -a 0x83c00000 -j 0x83c00000
            else
                sudo ${TOOLS_PATH}/usb-downloader -t nxp4330 \
                     -f ${RESULT_DIR}/fip-nonsecure-usb.bin -a 0x63c00000 -j 0x63c00000
            fi
        elif [ ${BOARD_PREFIX} == "bitminer" ]; then
            echo ${TOOLS_PATH}
            sudo ${TOOLS_PATH}/usb-downloader -t nxp4330 \
                 -b ${RESULT_DIR}/bl1-navi.bin \
                 -a 0xFFFF0000 -j 0xFFFF0000
            sleep 1
			sudo ${TOOLS_PATH}/usb-downloader -t nxp4330 \
				 -f ${RESULT_DIR}/fip-nonsecure-usb.bin -a 0x63c00000 -j 0x63c00000
        elif [ ${BOARD_POSTFIX} == "voice" ]; then
            sudo ${TOOLS_PATH}/usb-downloader -t nxp4330 \
                 -b ${RESULT_DIR}/bl1-${BOARD_PREFIX}_voice.bin \
                 -a 0xFFFF0000 -j 0xFFFF0000
            sleep 1
            if [ ${BOARD_PREFIX} == "ff" ]; then
                sudo ${TOOLS_PATH}/usb-downloader -t nxp4330 \
                     -f ${RESULT_DIR}/fip-nonsecure-usb.bin -a 0xA2000000 -j 0xA2000000
            else
                sudo ${TOOLS_PATH}/usb-downloader -t nxp4330 \
                     -f ${RESULT_DIR}/fip-nonsecure-usb.bin -a 0x83c00000 -j 0x83c00000
            fi

        else
            echo "Not supported board type"
        fi
    fi
}

get_board_prefix
run_by_usb
