#!/bin/bash

set -e

argc=$#
CURRENT_PATH=`dirname $0`
TOOLS_PATH=`readlink -ev $CURRENT_PATH`

RESULT_DIR="result-$1-$2"
RESULT_PATH=
IMAGE_TYPE=$2

MACHINE_NAME=$1
BOARD_SOCNAME=
BOARD_NAME=
BOARD_PREFIX=
BOARD_POSTFIX=

BUILD_ALL=$3

META_NEXELL_PATH=`readlink -ev ${TOOLS_PATH}/..`
BUILD_PATH=
TMP_DEPLOY_PATH=

function check_usage()
{
    if [ $argc -lt 3 ];then
	BUILD_ALL=true
    elif [ $argc -eq 3 ];then
        echo "Run at $PWD"
    else
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
    echo "    ex) $0 s5p4418-navi-ref genivi"
}

function split_args()
{
    BOARD_SOCNAME=${MACHINE_NAME%-*-*}
    BOARD_NAME=${MACHINE_NAME#*-}
    BOARD_PREFIX=${BOARD_NAME%-*}
    BOARD_POSTFIX=${BOARD_NAME#*-}
}

function path_setup()
{
    BUILD_PATH=`readlink -ev ${META_NEXELL_PATH}/../build-${MACHINE_NAME}-${IMAGE_TYPE}`
    RESULT_PATH=${BUILD_PATH}/../${RESULT_DIR}
    TMP_DEPLOY_PATH=${BUILD_PATH}/tmp/deploy/images/${MACHINE_NAME}
}

function cleanup_dirs()
{
    if [ ! -d ${RESULT_PATH} ];then
	mkdir -m 777 ${RESULT_PATH}
    else
	rm -rf ${RESULT_PATH}/boot
	rm -rf ${RESULT_PATH}/root
    fi
}

function copy_bin_files()
{
    if [ "${BOARD_SOCNAME}" == "s5p6818" ]; then
        if [ "${BOARD_NAME}" == "artik710-raptor" ]; then
	    cp ${TMP_DEPLOY_PATH}/bl1-raptor.bin ${RESULT_PATH}
        elif [ "${BOARD_NAME}" == "avn-ref" ]; then
            cp ${TMP_DEPLOY_PATH}/bl1-avn.bin ${RESULT_PATH}
        fi
        cp ${TMP_DEPLOY_PATH}/fip-loader.bin ${RESULT_PATH}
        cp ${TMP_DEPLOY_PATH}/fip-nonsecure.bin ${RESULT_PATH}
        cp ${TMP_DEPLOY_PATH}/fip-secure.bin ${RESULT_PATH}
    else
        if [ "${BOARD_NAME}" == "smart-voice" ]; then
            cp ${TMP_DEPLOY_PATH}/bl1-smart_voice.bin ${RESULT_PATH}
            cp ${META_NEXELL_PATH}/tools/${MACHINE_NAME}/bl1-smart-voice-usb.bin ${RESULT_PATH}
        else
            if [ "${BOARD_NAME}" == "avn-ref" ]; then
                cp ${META_NEXELL_PATH}/tools/${MACHINE_NAME}/bl1-avn-usb.bin ${RESULT_PATH}
            elif [ "${BOARD_NAME}" == "navi-ref" ]; then
                cp ${META_NEXELL_PATH}/tools/${MACHINE_NAME}/bl1-navi-usb.bin ${RESULT_PATH}
            elif [ "${BOARD_NAME}" == "daudio-ref" ]; then
                cp ${META_NEXELL_PATH}/tools/${MACHINE_NAME}/bl1-daudio-usb.bin ${RESULT_PATH}
            fi
            cp ${TMP_DEPLOY_PATH}/bl1-${BOARD_PREFIX}.bin ${RESULT_PATH}
        fi
        cp ${TMP_DEPLOY_PATH}/armv7_dispatcher.bin ${RESULT_PATH}
        cp ${TMP_DEPLOY_PATH}/pyrope-bl2.bin ${RESULT_PATH}
    fi
    cp ${TMP_DEPLOY_PATH}/bl1-emmcboot.bin ${RESULT_PATH}
    cp ${TMP_DEPLOY_PATH}/u-boot.bin ${RESULT_PATH}
    cp ${TMP_DEPLOY_PATH}/default_envs.txt ${RESULT_PATH}
}

function copy_kernel_image()
{
    if [ "${BOARD_SOCNAME}" == "s5p6818" ]; then
	cp ${TMP_DEPLOY_PATH}/Image ${RESULT_PATH}
    elif [ "${BOARD_SOCNAME}" == "s5p4418" ]; then
	cp ${TMP_DEPLOY_PATH}/zImage ${RESULT_PATH}
    else
	cp ${TMP_DEPLOY_PATH}/Image ${RESULT_PATH}
    fi
}

function copy_dtb_file()
{
    local file_name_dtb_rev0=
    local file_name_dtb_rev1=
    local kernel_image_path=

    if [ "${MACHINE_NAME}" == "s5p6818-artik710-raptor" ]; then
	file_name_dtb="s5p6818-artik710-raptor*.dtb"
	kernel_image_path=${BOARD_SOCNAME}_${BOARD_PREFIX}_${BOARD_POSTFIX}-poky-linux/linux-${MACHINE_NAME}
    elif [ "${MACHINE_NAME}" == "s5p6818-avn-ref" ]; then
	file_name_dtb="s5p6818-avn-ref*.dtb"
	kernel_image_path=${BOARD_SOCNAME}_${BOARD_PREFIX}_${BOARD_POSTFIX}-poky-linux/linux-${MACHINE_NAME}
    elif [ "${MACHINE_NAME}" == "s5p6818-kick-st" ]; then
	file_name_dtb="s5p6818-kick-st.dtb"
	kernel_image_path=${BOARD_SOCNAME}_${BOARD_PREFIX}_${BOARD_POSTFIX}-poky-linux/linux-${MACHINE_NAME}
    else
	kernel_image_path=${BOARD_SOCNAME}_${BOARD_PREFIX}_${BOARD_POSTFIX}-poky-linux-gnueabi/linux-${MACHINE_NAME}
	if [ ${BOARD_PREFIX} == "avn" ]; then
	    file_name_dtb="s5p4418-avn_ref*.dtb"
	elif [ ${BOARD_PREFIX} == "navi" ]; then
	    file_name_dtb="s5p4418-navi_ref*.dtb"
        elif [ ${BOARD_PREFIX} == "daudio" ]; then
	    file_name_dtb="s5p4418-daudio_ref*.dtb"
        elif [ ${BOARD_PREFIX} == "smart" ]; then
            file_name_dtb="s5p4418-smart_voice*.dtb"
        fi
    fi

    if [ ! -z "$file_name_dtb" -a "$file_name_dtb"!=" " ]; then
	find ${BUILD_PATH}/tmp/work/${kernel_image_path}/. -name ${file_name_dtb} -exec cp {} ${RESULT_PATH} \;

	#For local kernel source using
	exec < ${BUILD_PATH}/tmp/work/source_kernel_dir_path.txt

	read externalKernelPath
	find $externalKernelPath/. -name ${file_name_dtb} -exec cp {} ${RESULT_PATH} \;
    fi
}

function copy_ramdisk_image()
{
    if [ ${IMAGE_TYPE} != "tiny" ]; then
        cp ${META_NEXELL_PATH}/tools/${MACHINE_NAME}/uInitrd ${RESULT_PATH}
        #cp ${META_NEXELL_PATH}/tools/${MACHINE_NAME}/ramdisk_tiny.gz ${RESULT_PATH}
    fi
}

function copy_rootfs_image()
{
    if [ "${IMAGE_TYPE}" == "genivi" ];then
	cp ${TMP_DEPLOY_PATH}/"genivi-dev-platform-${MACHINE_NAME}.ext4" ${RESULT_PATH}
	cp ${RESULT_PATH}/"genivi-dev-platform-${MACHINE_NAME}.ext4" ${RESULT_PATH}/rootfs.img
    else
	cp ${TMP_DEPLOY_PATH}/"${MACHINE_NAME}-${IMAGE_TYPE}-${MACHINE_NAME}.tar.bz2" ${RESULT_PATH}
        cp ${TMP_DEPLOY_PATH}/"${MACHINE_NAME}-${IMAGE_TYPE}-${MACHINE_NAME}.ext4" ${RESULT_PATH}
        cp ${RESULT_PATH}/"${MACHINE_NAME}-${IMAGE_TYPE}-${MACHINE_NAME}.ext4" ${RESULT_PATH}/rootfs.img
    fi

    cp ${META_NEXELL_PATH}/tools/partition.txt ${RESULT_PATH}
}

function copy_params_image()
{
    cp ${META_NEXELL_PATH}/tools/${MACHINE_NAME}/params.bin ${RESULT_PATH}
}

function copy_partmap_file()
{
    cp ${META_NEXELL_PATH}/tools/${MACHINE_NAME}/partmap_emmc.txt ${RESULT_PATH}
}

function post_process()
{
    if [ -f secure.cfg ]; then
	cp secure.cfg ${RESULT_PATH}
        echo -e "\n secure.cfg file copy Done!"
    fi

    touch ${RESULT_PATH}/YOCTO.${RESULT_DIR}.INFO.DoNotChange
    echo -e "\033[40;33m  Maybe you need to convert some binary images                                \033[0m"
    echo -e "\033[40;33m  You can use below operation                                                 \033[0m"
    echo -e "\033[40;33m  ex) ${META_NEXELL_PATH}/tools/convert_images.sh ${MACHINE_NAME} ${IMAGE_TYPE}  \033[0m\n"
}

check_usage
split_args
path_setup
cleanup_dirs
copy_bin_files
copy_kernel_image
copy_dtb_file
#copy_modules_image
copy_ramdisk_image
if [ ${BUILD_ALL} == "true" ];then
    copy_rootfs_image
fi
#copy_params_image
copy_partmap_file

post_process
