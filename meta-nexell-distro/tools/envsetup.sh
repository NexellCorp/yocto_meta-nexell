#!/bin/bash

set -e

CURRENT_PATH=`dirname $0`
TOOLS_PATH=`readlink -ev $CURRENT_PATH`

# $1 ==> MACHINE_NAME : ex)s5p4418-navi-ref, s5p6818-avn-ref
# $2 ==> IMAGE_TYPE   : ex)tiny, qt, sato ...
argc=$#
PARENT_DIR="${PWD%/*}"

MACHINE_NAME=$1
IMAGE_TYPE=$2
NUMBER_THREADS=$3
EXTERNALSRC_USING=$4
QT_VERSION=$5

BOARD_SOCNAME=
BOARD_NAME=
BOARD_PREFIX=
BOARD_POSTFIX=

META_NEXELL_PATH=
NEXELL_BUILD_PATH=./

function check_usage()
{
    if [ $argc -lt 2 ]
    then
	echo "Invalid argument check usage please"
	usage
	exit
    fi
}

function usage()
{
    echo "Usage: $0 <machine-name> <image-type>"
    echo "Usage: $0 <machine-name> <image-type> <thread number:integer> <sdk:true or false> <external source:true or false>"
    echo "    ex) $0 s5p6818-avn-ref qt 8 false false"
    echo "    ex) $0 s5p4418-navi-ref qt -1 false true"
}

function split_args()
{
    BOARD_SOCNAME=${MACHINE_NAME%-*-*}
    BOARD_NAME=${MACHINE_NAME#*-}
    BOARD_PREFIX=${BOARD_NAME%-*}
    BOARD_POSTFIX=${BOARD_NAME#*-}
}

function target_validation_check()
{
    #Do not support target + imagetype
    if [ "${BOARD_SOCNAME}" == "s5p6818" ]; then
        #s5p6818 + X11 do not support
        if [ "${IMAGE_TYPE}" == "sato" ]; then
            echo -e "\033[0;33m *****************************************  \033[0m"
            echo -e "\033[0;33m libMali.so for X11 is not ready yet. \n So, This Build Can Not Try!! Sorry! \033[0m"
            echo -e "\033[0;33m *****************************************  \033[0m"
            exit 18
        fi

        if [ "${IMAGE_TYPE}" == "smartvoice" ]; then
            echo -e "\033[0;33m *****************************************  \033[0m"
            echo -e "\033[0;33m s5p6818 does not support smartvoice type \n So, This Build Can Not Try!! Sorry! \033[0m"
            echo -e "\033[0;33m *****************************************  \033[0m"
            exit 18
        fi

        if [ "${BOARD_NAME}" == "kick-st" -a "${IMAGE_TYPE}" != "qt" ]; then
            echo -e "\033[0;33m *****************************************  \033[0m"
            echo -e "\033[0;33m kick-st board support only qt type \n So, This Build Can Not Try!! Sorry! \033[0m"
            echo -e "\033[0;33m *****************************************  \033[0m"
            exit 18
        fi
    fi
}

function path_setup()
{
    META_NEXELL_PATH=`readlink -ev ${TOOLS_PATH}/..`

    if ! [ -d ${META_NEXELL_PATH}/../../build ]; then
        mkdir -p ${META_NEXELL_PATH}/../../build
    fi

    if ! [ -d ${META_NEXELL_PATH}/../../build/build-${MACHINE_NAME}-${IMAGE_TYPE} ]; then
        echo "Warning, please check -  source poky/oe-init-build-env build/build-<machine_name>-<image_type>"
        exit
    else
        NEXELL_BUILD_PATH=`readlink -ev ${META_NEXELL_PATH}/../../build/build-${MACHINE_NAME}-${IMAGE_TYPE}`
    fi
}

function customize_conf_files()
{
    cp ${META_NEXELL_PATH}/conf/local.conf.sample    ${NEXELL_BUILD_PATH}/conf/local.conf
    cp ${META_NEXELL_PATH}/conf/bblayers.conf.sample ${NEXELL_BUILD_PATH}/conf/bblayers.conf

    echo "#NEXELL appended code" >> ${NEXELL_BUILD_PATH}/conf/local.conf

    ${META_NEXELL_PATH}/tools/setup-conf-files.py ${NEXELL_BUILD_PATH} ${MACHINE_NAME} ${IMAGE_TYPE} ${META_NEXELL_PATH} ${EXTERNALSRC_USING} ${QT_VERSION}

    #-----------------------------------------------------------------
    # CORE THREADS numbers
    #-----------------------------------------------------------------
    if [ "${NUMBER_THREADS}" != "-1" ]; then
        echo "BB_NUMBER_THREADS = \"${NUMBER_THREADS}\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
    fi
}

function ready_extra_image()
{
    if [ "${IMAGE_TYPE}" == "ubuntu" ]; then
        cd ${NEXELL_BUILD_PATH}
        mkdir -p tmp/work/extra-rootfs-support
    fi
}

function copy_build_scripts()
{
    local secure=
    local TMP_WORK_PATH=${NEXELL_BUILD_PATH}/tmp/work

    if ! [ -d $TMP_WORK_PATH ];then
	mkdir -p $TMP_WORK_PATH
    fi
    
    #for secure boot support
    if [ "${BOARD_SOCNAME}" == "s5p6818" ]; then
        # If need to select secure ON option, write here 'secure_select' function call.
	# nexell yocto build secure option default off.
        echo "SECURE OFF" > ${NEXELL_BUILD_PATH}/secure.cfg; secure="OFF"
        python ${META_NEXELL_PATH}/tools/secure_tools/secure-setup.py ${secure} ${BOARD_SOCNAME} ${MACHINE_NAME} ${META_NEXELL_PATH}

    fi

    touch ${TMP_WORK_PATH}/SOURCE_PATH_FOR_OPTEE.txt
    touch ${TMP_WORK_PATH}/LINUX_STANDARD_BUILD_PATH.txt

    cp -a ${META_NEXELL_PATH}/tools/optee_pre_operation.sh ${NEXELL_BUILD_PATH}
    echo -e "\033[0;33m                                                                    \033[0m"
    echo -e "\033[0;33m #########  Start bitbake pre operateion for optee & ATF ########## \033[0m"
    echo -e "\033[0;33m                                                                    \033[0m"

    #-----------------------------------------------
    # bitbake pre-tasks process related optee & atf
    #-----------------------------------------------
    if [ ! -e ${NEXELL_BUILD_PATH}/OPTEE_PRE_OPERATION_DONE ];then
	${NEXELL_BUILD_PATH}/optee_pre_operation.sh ${MACHINE_NAME}
	touch ${NEXELL_BUILD_PATH}/OPTEE_PRE_OPERATION_DONE
    else
	echo -e "\033[0;33m #########  Already Done, optee & ATF pre-fetch & pre-unpack ########## \033[0m"
    fi

    #-----------------------------------------------
    # post process, for rootfs customize
    #-----------------------------------------------
    mkdir -p ${TMP_WORK_PATH}/use-post-process
}

function secure_select() {
    while :
    do
        if [ "$ERR_MSG" != "" ]; then
            echo "Error: $ERR_MSG"
            echo ""
        fi

        echo -e "\033[40;33m Artik710-raptor board support secure boot, Please select build option  \033[0m"
        echo -e "\033[40;33m     1:       Secure build  \033[0m"
        echo -e "\033[40;33m     2:   Non-Secure build  \033[0m"
        ERR_MSG=""

        read -p " Select : " SEL
        case $SEL in
            1) echo "SECURE ON" > secure.cfg; secure="ON"; break ;;
            2) echo "SECURE OFF" > secure.cfg; secure="OFF"; break ;;
            *) ERR_MSG="Please enter a valid option!"
        esac
    done
}

check_usage
split_args
target_validation_check
path_setup
customize_conf_files
ready_extra_image
copy_build_scripts

