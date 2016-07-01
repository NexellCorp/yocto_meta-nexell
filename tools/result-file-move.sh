#!/bin/bash

set -e

TOP=`pwd`
BOARD_NAME=$1
RESULT_DIR="result-$1"
PARENT_DIR="${PWD%/*}"
RESULT_PATH="${PARENT_DIR}/${RESULT_DIR}"
IMAGE_TYPE=$2

function check_result_dir()
{
    if [ -d "${RESULT_PATH}" ]
    then
        echo ""
    else    
        mkdir -p ${RESULT_PATH}	
    fi
}

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

function copy_kernel_images_avn-4418()
{
    cp ${TOP}/tmp/deploy/images/${BOARD_NAME}/Image ${RESULT_PATH}
    find ${TOP}/tmp/work/avn_4418-poky-linux-gnueabi/linux-avn-4418/. -name "s5p4418-avn_ref.dtb" -exec cp {} ${RESULT_PATH} \;
}

function copy_kernel_images_avn-6818()
{
    cp ${TOP}/tmp/deploy/images/${BOARD_NAME}/Image ${RESULT_PATH}
    find ${TOP}/tmp/work/avn_6818-poky-linux-gnueabi/linux-avn-6818/. -name "s5p6818-avn_ref.dtb" -exec cp {} ${RESULT_PATH} \;
}

function copy_rootfs_image()
{
    if [ ${IMAGE_TYPE} == "qt" -o ${IMAGE_TYPE} == "sato" ]
    then
        cp ${TOP}/../meta-nexell/tools/${BOARD_NAME}/ramdisk_tiny.gz ${RESULT_PATH}
    fi
    cp ${TOP}/tmp/deploy/images/${BOARD_NAME}/"nexell-${BOARD_NAME}-${IMAGE_TYPE}-${BOARD_NAME}.tar.bz2" ${RESULT_PATH}
    cp ${TOP}/tmp/deploy/images/${BOARD_NAME}/"nexell-${BOARD_NAME}-${IMAGE_TYPE}-${BOARD_NAME}.ext4" ${RESULT_PATH}
}

check_result_dir
copy_bin_files


if [ "${BOARD_NAME}" == "artik710-raptor" ]
then
    copy_kernel_images_artik710_raptor
elif [ "${BOARD_NAME}" == "avn-4418" ]
then
    copy_kernel_images_avn-4418
elif [ "${BOARD_NAME}" == "avn-6818" ]
then
    copy_kernel_images_avn-6818
fi    

copy_rootfs_image
