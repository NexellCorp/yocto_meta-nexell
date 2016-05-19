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

function copy_kernel_images_artik710_raptor()
{
    cp ${TOP}/tmp/deploy/images/${BOARD_NAME}/Image ${RESULT_PATH}
    find ${TOP}/tmp/work/artik710_raptor-poky-linux/linux-artik7/. -name "s5p6818-artik710-raptor-rev00.dtb" -exec cp {} ${RESULT_PATH} \;
    find ${TOP}/tmp/work/artik710_raptor-poky-linux/linux-artik7/. -name "s5p6818-artik710-raptor-rev01.dtb" -exec cp {} ${RESULT_PATH} \;
}

function copy_rootfs_image_tiny()
{
    cp ${TOP}/tmp/deploy/images/${BOARD_NAME}/"nexell-${BOARD_NAME}-tiny-${BOARD_NAME}.tar.bz2" ${RESULT_PATH}
}

copy_bin_files
copy_kernel_images_artik710_raptor
copy_rootfs_image_tiny
