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

TOP=`pwd`
export TOP

PARTMAP=
UPDATE_ALL=true
UPDATE_BL1=false
UPDATE_BOOTLOADER=false
UPDATE_ENV=false
UPDATE_BOOT=false
UPDATE_MODULES=false
UPDATE_ROOT=false
UPDATE_SYSTEM=false
UPDATE_DATA=false
RESULT_DIR=

function usage()
{
	echo "Usage: $0 -p <partmap-file> -r <result-dir> [-t bl1 -t bootloader -t env -t boot -t modules -t root -t system -t data -v]"
	echo -e '\n -p <partmap-file> : partmap file path'
	echo " -r <result-dir> : result dir path"
	echo " -t bl1\t: if you want to update only bl1, specify this, default no"
	echo " -t bootloader\t: if you want to update only bootloader, specify this, default no"
	echo " -t env\t: if you want to update only env, specify this, default no"
	echo " -t boot\t: if you want to update only boot partition, specify this, default no"
	echo " -t modules\t: if you want to update only modules partition, specify this, default no"
	echo " -t root\t: if you want to update only root partition, specify this, default no"
	echo " -t system\t: if you want to update only system partition, specify this, default no"
	echo " -t data\t: if you want to update only data partition, specify this, default no"
	echo " -v : if you want to view verboase build log message, specify this, default no"
}

function vmsg()
{
	local verbose=${VERBOSE:-"false"}
	if [ ${verbose} == "true" ]; then
		echo "$@"
	fi  
}

function parse_args()
{
	TEMP=`getopt -o "p:r:t:hv" -- "$@"`
	eval set -- "$TEMP"

	while true; do
		case "$1" in
			-p ) PARTMAP=$2; shift 2 ;;
			-r ) RESULT_DIR=$2; shift 2 ;;
			-t ) case "$2" in
				bl1    ) UPDATE_ALL=false; UPDATE_BL1=true ;;
				bootloader ) UPDATE_ALL=false; UPDATE_BOOTLOADER=true ;;
				env  ) UPDATE_ALL=false; UPDATE_ENV=true ;;
				boot ) UPDATE_ALL=false; UPDATE_BOOT=true ;;
				modules ) UPDATE_ALL=false; UPDATE_MODULES=true ;;
				system   ) UPDATE_ALL=false; UPDATE_SYSTEM=true ;;
				data ) UPDATE_ALL=false; UPDATE_DATA=true ;;
				root ) UPDATE_ALL=false; UPDATE_ROOT=true ;;
			     esac
			     shift 2 ;;
			-h ) usage; exit 1 ;;
			-v ) VERBOSE=true; shift 1 ;;
			-- ) break ;;
		esac
	done
}

function print_args()
{
	vmsg "================================================="
	vmsg " print args"
	vmsg "================================================="
	vmsg -e "PARTMAP:\t\t${PARTMAP}"
	vmsg -e "RESULT_DIR:\t\t${RESULT_DIR}"
	if [ ${UPDATE_ALL} == "true" ]; then
		vmsg -e "Update:\t\t\tAll"
	else
		if [ ${UPDATE_BL1} == "true" ]; then
			vmsg -e "Update:\t\t\tbl1"
		fi
		if [ ${UPDATE_BOOTLOADER} == "true" ]; then
			vmsg -e "Update:\t\t\tbootloader(fip files)"
		fi
		if [ ${UPDATE_ENV} == "true" ]; then
			vmsg -e "Update:\t\t\tenv"
		fi
		if [ ${UPDATE_BOOT} == "true" ]; then
			vmsg -e "Update:\t\t\tboot"
		fi
		if [ ${UPDATE_MODULES} == "true" ]; then
			vmsg -e "Update:\t\t\tmodules"
		fi
		if [ ${UPDATE_ROOT} == "true" ]; then
			vmsg -e "Update:\t\t\troot"
		fi
		if [ ${UPDATE_SYSTEM} == "true" ]; then
			vmsg -e "Update:\t\t\tsystem"
		fi
		if [ ${UPDATE_DATA} == "true" ]; then
			vmsg -e "Update:\t\t\tdata"
		fi
	fi
	vmsg
}

function flash()
{
	vmsg "flash $1 $2"
	sudo fastboot flash $1 $2
}

function update_partmap()
{
	local partmap=${1}
	if [ ! -f ${partmap} ]; then
		echo "partmap file -- ${partmap} -- no exist!!!"
		exit 0
	fi

	flash partmap ${partmap}
}

function update_bl1()
{
	if [ ${UPDATE_ALL} == "true" ] || [ ${UPDATE_BL1} == "true" ]; then
		local file=${1}
		vmsg "update bl1: ${file}"
		flash 2ndboot ${file}
	fi
}

function update_bootloader()
{
	if [ ${UPDATE_ALL} == "true" ] || [ ${UPDATE_BOOTLOADER} == "true" ]; then
		local file=${1}
		vmsg "update bootloader: ${file}"
		flash bootloader ${file}
	fi
}

function update_fip1()
{
	if [ ${UPDATE_ALL} == "true" ] || [ ${UPDATE_BOOTLOADER} == "true" ]; then
		local file=${1}
		vmsg "update loader: ${file}"
		flash fip-loader ${file}
	fi
}

function update_fip2()
{
	if [ ${UPDATE_ALL} == "true" ] || [ ${UPDATE_BOOTLOADER} == "true" ]; then
		local file=${1}
		vmsg "update secure: ${file}"
		flash fip-secure ${file}
	fi
}

function update_fip3()
{
	if [ ${UPDATE_ALL} == "true" ] || [ ${UPDATE_BOOTLOADER} == "true" ]; then
		local file=${1}
		vmsg "update nonsecure: ${file}"
		flash fip-nonsecure ${file}
	fi
}

function update_env()
{
	if [ ${UPDATE_ALL} == "true" ] || [ ${UPDATE_ENV} == "true" ]; then
		local file=${1}
		vmsg "update env: ${file}"
		flash env ${file}
	fi
}

function update_boot()
{
	if [ ${UPDATE_ALL} == "true" ] || [ ${UPDATE_BOOT} == "true" ]; then
		local file=${1}
		vmsg "update boot: ${file}"
		flash boot ${file}
	fi
}

function update_modules()
{
	if [ ${UPDATE_ALL} == "true" ] || [ ${UPDATE_MODULES} == "true" ]; then
		local file=${1}
		vmsg "update modules: ${file}"
		flash modules ${file}
	fi
}

function update_root()
{
	if [ ${UPDATE_ALL} == "true" ] || [ ${UPDATE_ROOT} == "true" ]; then
		local file=${1}
		vmsg "update rootfs: ${file}"
		flash rootfs ${file}
	fi
}

function update_system()
{
	if [ ${UPDATE_ALL} == "true" ] || [ ${UPDATE_SYSTEM} == "true" ]; then
		local file=${1}
		vmsg "update system: ${file}"
		flash system ${file}
	fi
}

function update_data()
{
	if [ ${UPDATE_ALL} == "true" ] || [ ${UPDATE_DATA} == "true" ]; then
		local file=${1}
		vmsg "update data: ${file}"
		flash user ${file}
	fi
}

parse_args $@
print_args
update_partmap ${PARTMAP}
update_bl1 ${RESULT_DIR}/bl1-emmcboot.img
update_fip1 ${RESULT_DIR}/fip-loader-emmc.img
update_fip2 ${RESULT_DIR}/fip-secure.img
update_fip3 ${RESULT_DIR}/fip-nonsecure.img
update_env ${RESULT_DIR}/params.bin
update_boot ${RESULT_DIR}/boot.img
update_modules ${RESULT_DIR}/modules.img
update_root ${RESULT_DIR}/rootfs.img
# update_system ${RESULT_DIR}/system.img
# update_data ${RESULT_DIR}/user.img
