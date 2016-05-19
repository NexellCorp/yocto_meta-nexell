#!/bin/bash

set -e

TOP=`pwd`
BOARD_NAME=$1
RESULT_DIR="result-$1"
PARENT_DIR="${PWD%/*}"
RESULT_PATH="${PARENT_DIR}/${RESULT_DIR}"

function copy_bin_files()
{
    python ${PARENT_DIR}/meta-nexell/tools/result-file-move.py "${TOP}/tmp/work/image_where.txt"
}

function copy_kernel_images()
{
    cp ${TOP}/tmp/deploy/images/${BOARD_NAME}/Image ${RESULT_PATH}
    cp ${TOP}/tmp/work/artik710_raptor-poky-linux/linux-artik7/1_4.1.0322-r0/linux-artik710_raptor-standard-build/arch/arm64/boot/dts/nexell/s5p6818-artik710-raptor-rev00.dtb ${RESULT_PATH}
    cp ${TOP}/tmp/work/artik710_raptor-poky-linux/linux-artik7/1_4.1.0322-r0/linux-artik710_raptor-standard-build/arch/arm64/boot/dts/nexell/s5p6818-artik710-raptor-rev01.dtb ${RESULT_PATH}
}

function copy_rootfs_image()
{
    cp ${TOP}/tmp/deploy/images/${BOARD_NAME}/"nexell-${BOARD_NAME}-image-${BOARD_NAME}.tar.bz2" ${RESULT_PATH}
}

copy_bin_files
copy_kernel_images
copy_rootfs_image

chdir(${RESULT_PATH})
