#!/bin/bash

set -e

argc=$#
PARENT_DIR="${PWD%/*}"
ROOTDIR="root"
BOOTDIR="boot"
META_NEXELL_TOOLS_DIR="${PARENT_DIR}/meta-nexell/tools"
IMAGE_TYPE=$1

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
    echo "Usage: $0 <image-type>"
    echo "    ex) $0 tiny"
    echo "    ex) $0 sato"
    echo "    ex) $0 qt"
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

function mkbootimg()
{
    ${META_NEXELL_TOOLS_DIR}/mkimage -A arm64 -O linux -T kernel -C none -a 0x40080000 -e 0x40080000 -n 'linux-4.1' -d Image ./boot/uImage
    mv ramdisk.gz ./boot
    mv *.dtb ./boot
    ${META_NEXELL_TOOLS_DIR}/make_ext4fs -b 4096 -L boot -l 33554432 boot.img ./boot/    
}

check_usage
make_dirs
mkramdisk
mkbootimg
