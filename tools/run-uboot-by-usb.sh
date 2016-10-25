#!/bin/bash

set -e

argc=$#
PARENT_DIR="${PWD%/*}"
META_NEXELL_TOOLS_DIR="${PARENT_DIR}/meta-nexell/tools"
MACHINE_NAME=$1
BOARD_SOCNAME=
BOARD_NAME=
BOARD_PREFIX=
BOARD_POSTFIX=

function check_usage()
{
    if [ $argc != 1 ]
    then
	echo "Invalid argument check usage please"
	usage
	exit
    fi
}

function usage()
{
    echo "Usage: $0 <machine-name>"
    echo "    ex) $0 s5p6818-artik710-raptor"
    echo "    ex) $0 s5p6818-avn-ref"
    echo "    ex) $0 s5p4418-avn-ref"
    echo "    ex) $0 s5p4418-navi-ref"
}

function get_board_prefix()
{
    BOARD_SOCNAME=${MACHINE_NAME%-*-*}
    BOARD_NAME=${MACHINE_NAME#*-}
    BOARD_PREFIX=${BOARD_NAME%-*}
    BOARD_POSTFIX=${BOARD_NAME#*-}
}

function run_by_usb()
{
	if [ ${BOARD_SOCNAME} == "s5p6818" ]; then
		if [ "${BOARD_NAME}" == "artik710-raptor" ]; then
			sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t slsiap \
				-n ${META_NEXELL_TOOLS_DIR}/${MACHINE_NAME}/raptor-64.txt \
				-b ${META_NEXELL_TOOLS_DIR}/${MACHINE_NAME}/bl1-raptor-64.bin
			sleep 1
			sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t slsiap \
				-n ${META_NEXELL_TOOLS_DIR}/${MACHINE_NAME}/raptor-64.txt \
				-f u-boot.bin -a 0x43c00000 -j 0x43c00000
		elif [ "${BOARD_NAME}" == "avn-ref" ]; then
			sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t slsiap \
				-n ${META_NEXELL_TOOLS_DIR}/${MACHINE_NAME}/nsih_avn_ref_usb.txt \
				-b bl1-avn.bin
			sleep 1
			sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t slsiap \
				-f fip-loader-usb.img -m
		fi
	else
		if [ ${BOARD_PREFIX} == "avn" ]; then
			sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t slsiap \
				-n ${META_NEXELL_TOOLS_DIR}/${MACHINE_NAME}/nsih_${BOARD_PREFIX}_ref_usb.txt \
				-b bl1-${BOARD_PREFIX}.bin
			sleep 1
			sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t slsiap \
				-n ${META_NEXELL_TOOLS_DIR}/${MACHINE_NAME}/nsih_${BOARD_PREFIX}_ref_usb.txt \
				-f u-boot.bin -a 0x43c00000 -j 0x43c00000
		elif [ ${BOARD_PREFIX} == "navi" ]; then
			sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t nxp4330 \
				-n ${META_NEXELL_TOOLS_DIR}/${MACHINE_NAME}/nsih_${BOARD_PREFIX}_ref_usb.txt \
				-b bl1-${BOARD_PREFIX}-usb.bin
			sleep 1
			sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t nxp4330 \
				-n ${META_NEXELL_TOOLS_DIR}/${MACHINE_NAME}/nsih_${BOARD_PREFIX}_ref_usb.txt \
				-f u-boot.bin -a 0x43c00000 -j 0x43c00000
		else
			echo "Not supported board type"
		fi
	fi
}

check_usage
get_board_prefix
run_by_usb
