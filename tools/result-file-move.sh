#!/bin/bash

set -e

argc=$#
TOP=`pwd`
RESULT_DIR="result-$1"
PARENT_DIR="${PWD%/*}"
RESULT_PATH="${PARENT_DIR}/${RESULT_DIR}"
IMAGE_TYPE=$2

MACHINE_NAME=$1
BOARD_SOCNAME=
BOARD_NAME=
BOARD_PREFIX=
BOARD_POSTFIX=

function check_result_dir()
{
    if [ -d "${RESULT_PATH}" ]
    then
        echo ""
    else    
        mkdir -p ${RESULT_PATH}	
    fi
}

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
}

function get_board_prefix()
{
    BOARD_SOCNAME=${MACHINE_NAME%-*-*}
    BOARD_NAME=${MACHINE_NAME#*-}
    BOARD_PREFIX=${BOARD_NAME%-*}
    BOARD_POSTFIX=${BOARD_NAME#*-}
}

function copy_bin_files()
{
    python ${PARENT_DIR}/meta-nexell/tools/result-file-move.py "${TOP}/tmp/work/image_where.txt"
}

function copy_kernel_image()
{
    cp ${TOP}/tmp/deploy/images/${MACHINE_NAME}/Image ${RESULT_PATH}
}

function copy_dtb_file()
{
    local file_name_dtb_rev0=
    local file_name_dtb_rev1=
    local kernel_image_path=
    
    if [ "${BOARD_NAME}" == "artik710-raptor" ]; then
	file_name_dtb_rev0="s5p6818-artik710-raptor-rev00.dtb"
	file_name_dtb_rev1="s5p6818-artik710-raptor-rev01.dtb"
	kernel_image_path=${BOARD_SOCNAME}_${BOARD_PREFIX}_${BOARD_POSTFIX}-poky-linux/linux-${MACHINE_NAME}
    else
	kernel_image_path="${BOARD_SOCNAME}_${BOARD_PREFIX}_${BOARD_POSTFIX}-poky-linux-gnueabi/linux-${MACHINE_NAME}"
	if [ ${BOARD_PREFIX} == "avn" ]; then
	    file_name_dtb_rev0="s5p4418-avn_ref-rev00.dtb"
	elif [ ${BOARD_PREFIX} == "navi" ]; then
	    file_name_dtb_rev0="s5p4418-navi_ref-rev00.dtb"
	fi
    fi

    if [ ! -z "$file_name_dtb_rev0" -a "$file_name_dtb_rev0"!=" " ]; then
	find ${TOP}/tmp/work/${kernel_image_path}/. -name ${file_name_dtb_rev0} -exec cp {} ${RESULT_PATH} \;
    fi

    if [ ! -z "$file_name_dtb_rev1" -a "$file_name_dtb_rev1"!=" " ]; then
	find ${TOP}/tmp/work/${kernel_image_path}/. -name ${file_name_dtb_rev1} -exec cp {} ${RESULT_PATH} \;
    fi
}

function copy_rootfs_image()
{
    if [ ${IMAGE_TYPE} == "qt" -o ${IMAGE_TYPE} == "sato" ]; then
        cp ${TOP}/../meta-nexell/tools/${MACHINE_NAME}/ramdisk_tiny.gz ${RESULT_PATH}
    fi
    cp ${TOP}/tmp/deploy/images/${MACHINE_NAME}/"${MACHINE_NAME}-${IMAGE_TYPE}-${MACHINE_NAME}.tar.bz2" ${RESULT_PATH}
    cp ${TOP}/tmp/deploy/images/${MACHINE_NAME}/"${MACHINE_NAME}-${IMAGE_TYPE}-${MACHINE_NAME}.ext4" ${RESULT_PATH}
    cp ${RESULT_PATH}/"${MACHINE_NAME}-${IMAGE_TYPE}-${MACHINE_NAME}.ext4" ${RESULT_PATH}/rootfs.img
}

function copy_params_image()
{
    cp ${TOP}/../meta-nexell/tools/${MACHINE_NAME}/params.bin ${RESULT_PATH}
}

function copy_partmap_file()
{
    cp ${TOP}/../meta-nexell/tools/${MACHINE_NAME}/partmap_emmc.txt ${RESULT_PATH}
}

function post_process()
{
    echo -e "\033[40;33m  Maybe you need to convert some binary images                                \033[0m"
    echo -e "\033[40;33m  You can use below operation                                                 \033[0m"
    echo -e "\033[40;33m  ex) $ ../meta-nexell/tools/convert_images.sh ${MACHINE_NAME} ${IMAGE_TYPE}  \033[0m\n"
}

check_usage
check_result_dir
get_board_prefix
copy_bin_files
copy_kernel_image
copy_dtb_file
copy_rootfs_image
copy_params_image
copy_partmap_file

post_process
