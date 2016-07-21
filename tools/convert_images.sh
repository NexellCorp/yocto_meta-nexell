#!/bin/bash

set -e

argc=$#
PARENT_DIR="${PWD%/*}"
ROOTDIR="root"
BOOTDIR="boot"
META_NEXELL_TOOLS_DIR="${PARENT_DIR}/meta-nexell/tools"
IMAGE_TYPE=$2
MEM_SIZE=1024
MACHINE_NAME=$1
BOARD_SOCNAME=
BOARD_NAME=
BOARD_PREFIX=
BOARD_POSTFIX=

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
    echo "Usage: $0 <machine name> <image type>"
    echo "    ex) $0 s5p6818-artik710-raptor qt"
    echo "    ex) $0 s5p6818-artik710-raptor tiny"
    echo "    ex) $0 s5p4418-avn-ref qt"
    echo "    ex) $0 s5p4418-avn-ref tiny"
    echo "    ex) $0 s5p4418-navi-ref qt"
    echo "    ex) $0 s5p4418-navi-ref tiny"
    echo "    ex) $0 s5p4418-navi-ref tinyui"
}

function get_board_prefix()
{
    BOARD_SOCNAME=${MACHINE_NAME%-*-*}
    BOARD_NAME=${MACHINE_NAME#*-}
    BOARD_PREFIX=${BOARD_NAME%-*}
    BOARD_POSTFIX=${BOARD_NAME#*-}
}

function make_dirs()
{
    rm -rf ${BOOTDIR}
    rm -rf ${ROOTDIR}
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
    if [ ${IMAGE_TYPE} != "tiny" ]
    then
	mv ramdisk_tiny.gz ramdisk.gz
    else
	mv *.tar.bz2 ${ROOTDIR}
	cd ${ROOTDIR}
	tar -xvf *.tar.bz2
	rm *.tar.bz2
	cd ..
	
	dd if=/dev/zero of=$ROOTDIR.img bs=1M count=16
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
    local chip_name=${BOARD_SOCNAME}
    local gen_img=bl1-emmcboot.bin
    local aes_out_img=bl1-emmcboot.img
    local bootbingen=BOOT_BINGEN

    # BINGEN
    if [ "${BOARD_NAME}" == "artik710-raptor" ]; then
        bl1_source=bl1-${BOARD_POSTFIX}.bin
        file_name=raptor-emmc-32.txt
    else
        bl1_source=bl1-${BOARD_PREFIX}.bin
        file_name=nsih_${BOARD_PREFIX}_ref_emmc.txt
    fi

    local nsih=${PARENT_DIR}/meta-nexell/tools/${MACHINE_NAME}/${file_name}

    if [ "${MACHINE_NAME}" == "s5p4418-navi-ref" ]; then
        chip_name="nxp4330"
        bootbingen=BOOT_BINGEN_NAVI
    fi

    ${PARENT_DIR}/meta-nexell/tools/${bootbingen} -c ${chip_name} -t 2ndboot -n ${nsih} -i ${bl1_source} -o ${gen_img} -l 0xffff0000 -e 0xffff0000
}

function make_3rdboot_for_emmc()
{
    local file_name=
    local inout_image=u-boot
    local chip_name=${BOARD_SOCNAME}

    if [ "${BOARD_NAME}" == "artik710-raptor" ]; then
        file_name=raptor-emmc-32.txt
    else
        file_name=nsih_${BOARD_PREFIX}_ref_emmc.txt
    fi
    
    local nsih=${PARENT_DIR}/meta-nexell/tools/${MACHINE_NAME}/${file_name}
 
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

    if [ "${MACHINE_NAME}" == "s5p4418-navi-ref" ]; then
        chip_name="nxp4330"
    fi
 
    ${PARENT_DIR}/meta-nexell/tools/BOOT_BINGEN -c ${chip_name} -t 3rdboot -n ${nsih} -i ${inout_image}.bin -o singleimage-emmcboot.bin -l ${load_addr} -e ${jump_addr}
}


function mkbootimg()
{
    if [ "${BOARD_NAME}" == "artik710-raptor" ]
    then
        ${META_NEXELL_TOOLS_DIR}/mkimage -A arm64 -O linux -T kernel -C none -a 0x40080000 -e 0x40080000 -n 'linux-4.1' -d Image ./boot/uImage
    elif [ "${BOARD_PREFIX}" == "avn" ]
    then
        ${META_NEXELL_TOOLS_DIR}/mkimage -A arm -O linux -T kernel -C none -a 0x40008000 -e 0x40008000 -n 'linux-4.1' -d Image ./boot/uImage
    elif [ "${BOARD_PREFIX}" == "navi" ]
    then
        ${META_NEXELL_TOOLS_DIR}/mkimage -A arm -O linux -T kernel -C none -a 0x40008000 -e 0x40008000 -n 'linux-4.1' -d Image ./boot/uImage
    else
        echo -e "error"
    fi

    mv ramdisk.gz ./boot
    mv *.dtb ./boot
    ${META_NEXELL_TOOLS_DIR}/make_ext4fs -b 4096 -L boot -l 33554432 boot.img ./boot/    
}

function post_process()
{
    echo -e "\n\n\033[40;33m  Target download method.....                                                        \033[0m\n"
    if  [ "${MACHINE_NAME}" == "s5p6818-artik710-raptor" ]; then
        echo -e "\033[40;33m  <Full download>                                                                        \033[0m"
        echo -e "\033[40;33m      ex) $ ../meta-nexell/tools/update_s5p6818_artik710-raptor.sh -p partmap_emmc.txt -r .  \033[0m"
        echo -e "\033[40;33m  <kernel only>                                                                          \033[0m"
        echo -e "\033[40;33m      ex) $ ../meta-nexell/tools/update_s5p6818_artik710-raptor_kernel_only.sh               \033[0m"
        echo -e "\033[40;33m  <rootfs.img>                                                                           \033[0m"
        echo -e "\033[40;33m      ex) $ sudo fastboot flash rootfs rootfs.img                                            \033[0m\n"
    else
        echo -e "\033[40;33m  <Full download>                                                        \033[0m"
        echo -e "\033[40;33m      ex) $ ../meta-nexell/tools/update_s5p4418.sh -p partmap_emmc.txt -r .  \033[0m"
        echo -e "\033[40;33m  <bl1, u-boot, kernel only>                                             \033[0m"
        echo -e "\033[40;33m      ex) $ ../meta-nexell/tools/update_s5p4418_kernel_uboot_bl1_only.sh     \033[0m"
        echo -e "\033[40;33m  <rootfs.img>                                                           \033[0m"
        echo -e "\033[40;33m      ex) $ sudo fastboot flash rootfs rootfs.img                            \033[0m\n"
    fi
}

check_usage
get_board_prefix
make_dirs
mkramdisk
mkbootimg
make_2ndboot_for_emmc
make_3rdboot_for_emmc
post_process
