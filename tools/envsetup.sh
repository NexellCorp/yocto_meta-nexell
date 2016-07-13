#!/bin/bash

set -e

argc=$#
RESULT_DIR="result-$1"
PARENT_DIR="${PWD%/*}"
RESULT_PATH="${PARENT_DIR}/${RESULT_DIR}"
IMAGE_TYPE=$2
NEXELL_CODE_MASK="NEXELL appended code"

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
    echo "Usage: $0 <machine-name> <image-type>"
    echo "    ex) $0 s5p6818-artik710-raptor tiny"
    echo "    ex) $0 s5p6818-artik710-raptor sato"
    echo "    ex) $0 s5p6818-artik710-raptor qt"
    echo "    ex) $0 s5p4418-avn-ref qt"
    echo "    ex) $0 s5p4418-avn-ref tiny"
    echo "    ex) $0 s5p4418-navi-ref qt"
    echo "    ex) $0 s5p4418-navi-ref tiny"
}

function make_result_dir()
{
    BOARD_SOCNAME=${MACHINE_NAME%-*-*}
    BOARD_NAME=${MACHINE_NAME#*-}
    BOARD_PREFIX=${BOARD_NAME%-*}
    BOARD_POSTFIX=${BOARD_NAME#*-}

    sudo rm -rf "${PARENT_DIR}/${RESULT_DIR}"
    sudo mkdir -m 777 "${PARENT_DIR}/${RESULT_DIR}"
}

function customize_conf_files()
{
    cp ${PARENT_DIR}/meta-nexell/misc/local.conf.org ./conf/local.conf
    cp ${PARENT_DIR}/meta-nexell/misc/bblayers-${IMAGE_TYPE}-sample.~conf ./conf/bblayers.conf
    ${PARENT_DIR}/meta-nexell/tools/setup-conf-files.py ${MACHINE_NAME} ${IMAGE_TYPE}
    echo ${RESULT_PATH} > result_path.txt
}

function customize_recipe_core_files()
{
    if [ "${IMAGE_TYPE}" == "tiny" -o "${IMAGE_TYPE}" == "qt" -o "${IMAGE_TYPE}" == "sato" ]
    then
	${PARENT_DIR}/meta-nexell/tools/recipes-core-filename-change.py ${MACHINE_NAME} ${IMAGE_TYPE}
    else
	usage
    fi
}

function copy_build_scripts()
{
    #temp ARM 32bit build toolchain copy
    cp -a ${PARENT_DIR}/meta-nexell/tools/toolchain_setup.sh .
    ./toolchain_setup.sh

    mkdir -p tmp/work 
    cp -a ${PARENT_DIR}/meta-nexell/tools/bitbake_pre_operation_${MACHINE_NAME}.sh .
    ./bitbake_pre_operation_${MACHINE_NAME}.sh

    touch tmp/work/source_dir_path.txt

    echo -e "\033[40;33m                                                        \033[0m"
    echo -e "\033[40;33m You are now ready to run the bitbake command for NEXELL\033[0m"
    echo -e "\033[40;33m                                                        \033[0m"
    echo -e "\033[40;33m  ex) $ bitbake ${MACHINE_NAME}-${IMAGE_TYPE}           \033[0m\n\n"
}

check_usage
make_result_dir
customize_conf_files
customize_recipe_core_files
copy_build_scripts
