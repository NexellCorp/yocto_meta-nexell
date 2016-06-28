#!/bin/bash

set -e

argc=$#
PARENT_DIR="${PWD%/*}"
ROOTDIR="root"
BOOTDIR="boot"
META_NEXELL_TOOLS_DIR="${PARENT_DIR}/meta-nexell/tools"
BOARD_NAME=$1
IMAGE_TYPE=$2
MEM_SIZE=1024

function check_usage()
{
    if [ $argc != 2 ]
    then
	echo "Invalid argument check usage please"
	usage
	exit
    fi
}

function usage()
{
    echo "Usage: $0 <board name> <image-type>"
    echo "    ex) $0 artik710-raptor tiny"
    echo "    ex) $0 artik710-raptor sato"
    echo "    ex) $0 avn qt"
}

function make_dirs()
{
    mkdir -m 777 ${BOOTDIR}
    mkdir -m 777 ${ROOTDIR}
}

function find_loop_device()
{
	local loop_dev=
	loop_dev=$(sudo losetup -a | grep ${ROOTDIR}.img | awk '{print $1}') && echo ${loop_dev%:*}
}

function mkramdisk()
{
    if [ ${IMAGE_TYPE} == "qt" -o ${IMAGE_TYPE} == "sato" ]
    then
	mv ramdisk_tiny.gz ramdisk.gz
    else
	mv *.tar.bz2 ${ROOTDIR}
	cd ${ROOTDIR}
	tar -xvf *.tar.bz2
	rm *.tar.bz2
	cd ..
	
	dd if=/dev/zero of=$ROOTDIR.img bs=1M count=32
	sudo losetup -f $ROOTDIR.img
	LOOP_DEVICE=$(find_loop_device)
	sudo mkfs.ext2 ${LOOP_DEVICE}
	sleep 1
	mkdir -p mnt
	sudo mount -t ext2 -o loop ${LOOP_DEVICE} mnt
	sudo cp -a $ROOTDIR/* mnt/
	sleep 1
	sudo umount mnt
	sleep 1
	sudo losetup -d ${LOOP_DEVICE}
	rm -rf mnt
	gzip -9 $ROOTDIR.img

	mv root.img.gz ramdisk.gz
    fi
}

function make_2ndboot_for_emmc()
{
    local bl1_source=
    local file_name=
    local chip_name=
    local gen_img=bl1-emmcboot.bin
    local aes_out_img=bl1-emmcboot.img

    # BINGEN
    if [ "${BOARD_NAME}" == "artik710-raptor" ]; then
        bl1_source=bl1-artik7
        file_name=raptor-emmc-32.txt
	chip_name=s5p6818
    elif [ "${BOARD_NAME}" == "avn" ]; then
        bl1_source=bl1-avn
        file_name=nsih_avn_ref_emmc.txt
	chip_name=s5p4418
    fi

    local nsih=${PARENT_DIR}/meta-nexell/tools/${BOARD_NAME}/${file_name}

    ${PARENT_DIR}/meta-nexell/tools/BOOT_BINGEN -c ${chip_name} -t 2ndboot -n ${nsih} -i bl1-${BOARD_NAME}.bin -o ${gen_img} -l 0xffff0000 -e 0xffff0000
}

function make_3rdboot_for_emmc()
{
    local bl1_source=
    local file_name=
    local inout_image=
    local chip_name=

    if [ "${BOARD_NAME}" == "artik710-raptor" ]; then
        bl1_source=bl1-artik7
        file_name=raptor-emmc-32.txt
	chip_name=s5p6818
        inout_image=u-boot
    elif [ "${BOARD_NAME}" == "avn" ]; then
        bl1_source=bl1-avn
        file_name=nsih_avn_ref_emmc.txt
	chip_name=s5p4418
        inout_image=u-boot
    else
        bl1_source=bl1-artik530
        file_name=${BOARD_PURENAME}-emmc.txt
        inout_image=u-boot
    fi

    local nsih=${PARENT_DIR}/meta-nexell/tools/${BOARD_NAME}/${file_name}
 
    local load_addr=
    local jump_addr=
    if [ "${BOARD_NAME}" == "artik710-raptor" ]; then
        case "${MEM_SIZE}" in
            512)  load_addr=0x5fc00000; jump_addr=0x5fe00000 ;;
            1024) load_addr=0x7fc00000; jump_addr=0x7fe00000 ;;
            2048) load_addr=0xbfc00000; jump_addr=0xbfe00000 ;;
        esac
    else
        load_addr=0x43c00000
        jump_addr=0x43c00000
    fi
 
    ${PARENT_DIR}/meta-nexell/tools/BOOT_BINGEN -c ${chip_name} -t 3rdboot -n ${nsih} -i ${inout_image}.bin -o singleimage-emmcboot.bin -l ${load_addr} -e ${jump_addr}
}


function mkbootimg()
{
    if [ "${BOARD_NAME}" == "artik710-raptor" ]
    then
        ${META_NEXELL_TOOLS_DIR}/mkimage -A arm64 -O linux -T kernel -C none -a 0x40080000 -e 0x40080000 -n 'linux-4.1' -d Image ./boot/uImage
    elif [ "${BOARD_NAME}" == "avn" ]
    then
        ${META_NEXELL_TOOLS_DIR}/mkimage -A arm -O linux -T kernel -C none -a 0x40008000 -e 0x40008000 -n 'linux-4.1' -d Image ./boot/uImage
    else
        echo -e "error"
    fi

    mv ramdisk.gz ./boot
    mv *.dtb ./boot
    ${META_NEXELL_TOOLS_DIR}/make_ext4fs -b 4096 -L boot -l 33554432 boot.img ./boot/    
}

check_usage
make_dirs
mkramdisk
mkbootimg
make_2ndboot_for_emmc
make_3rdboot_for_emmc
