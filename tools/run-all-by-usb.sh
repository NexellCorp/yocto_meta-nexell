#!/bin/bash

# Copyright (c) 2016 Nexell Co., Ltd.
# Author: Sungwoo, Park <swpark@nexell.co.kr>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

set -e

argc=$#
PARENT_DIR="${PWD%/*}"
META_NEXELL_TOOLS_DIR="${PARENT_DIR}/meta-nexell/tools"
MACHINE_NAME=$1
BOARD_SOCNAME=
BOARD_NAME=
BOARD_PREFIX=
BOARD_POSTFIX=

MEM_SIZE=1024
FILE_NAME=singleimage.bin

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
    echo "    ex) $0 s5p4418-avn-ref"
    echo "    ex) $0 s5p4418-navi-ref"
    echo "    ex) $0 s5p4418-hs-iot"
}

function get_board_prefix()
{
    BOARD_SOCNAME=${MACHINE_NAME%-*-*}
    BOARD_NAME=${MACHINE_NAME#*-}
    BOARD_PREFIX=${BOARD_NAME%-*}
    BOARD_POSTFIX=${BOARD_NAME#*-}
}

function run_all_by_usb()
{
    if [ ${BOARD_SOCNAME} == "s5p6818" ]; then
        NSIH_SUFFIX=-32;
	BL_SOURCE=bl1-artik7
	TYPE=slsiap
    else
	NSIH_SUFFIX=
	BL_SOURCE=bl1-artik530
	MEM_SIZE=512
	DOWN_ADDR=0x43c00000
	JUMP_ADDR=0x43c00000
	FILE_NAME=u-boot.bin
	TYPE=
    fi


    if [ "${BL_SOURCE}" == "bl1-artik7" ]; then
	case "${MEM_SIZE}" in
		512)  DOWN_ADDR=0x5fc00000; JUMP_ADDR=0x5fe00000 ;;
		1024) DOWN_ADDR=0x7fc00000; JUMP_ADDR=0x7fe00000 ;;
		2048) DOWN_ADDR=0xbfc00000; JUMP_ADDR=0xbfe00000 ;;
	esac
    fi
    
    sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t ${TYPE} -n ${META_NEXELL_TOOLS_DIR}/${MACHINE_NAME}/raptor-32.txt -b bl1-raptor.bin
    sleep 1
    sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t slsiap -n ${META_NEXELL_TOOLS_DIR}/${MACHINE_NAME}/raptor-32.txt -f singleimage-emmcboot.bin -a ${DOWN_ADDR} -j ${JUMP_ADDR}

    echo "copy and paste following command to serial terminal after ${BOARD_PREFIX}# "
    echo
    echo '"udown 0x48000000;udown 0x49000000;udown 0x4a000000"'
    echo

    echo "If you are ready, type enter"
    read

    sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t slsiap -f boot/uImage
    sleep 1
    sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t slsiap -f boot/ramdisk.gz
    sleep 1
    sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t slsiap -f boot/s5p6818-artik710-raptor-rev01.dtb
    sleep 1

    echo "If you can see ${BOARD_PREFIX}# , copy and paste following command to serial terminal and enter" 
    echo
    echo '"bootm 0x48000000 - 0x4a000000"'
}


check_usage
get_board_prefix
run_all_by_usb
