#!/bin/bash

set -e

argc=$#
BOARD_NAME=$1
RESULT_DIR="result-$1"
PARENT_DIR="${PWD%/*}"
RESULT_PATH="${PARENT_DIR}/${RESULT_DIR}"
IMAGE_TYPE=$2

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
    echo "Usage: $0 <board-name> <image-type>"
    echo "    ex) $0 artik710-raptor tiny"
    echo "    ex) $0 artik710-raptor sato"
    echo "    ex) $0 artik710-raptor qt"
}

function make_result_dir()
{
    BOARD_PURENAME=${BOARD_NAME#*-}
    BOARD_PREFIX=${BOARD_NAME%-*}
    echo "BOARD_PURENAME --> ${BOARD_PURENAME}"
    echo "BOARD_PREFIX --> ${BOARD_PREFIX}"
    mkdir -m 777 "${PARENT_DIR}/${RESULT_DIR}"
}

function customize_conf_files()
{
    ${PARENT_DIR}/meta-nexell/tools/setup-conf-files.py ${BOARD_NAME} ${IMAGE_TYPE}
    echo ${RESULT_PATH} > result_path.txt
}

function customize_recipe_core_files()
{
    echo -e "\033[44;33m some recipe files name change in meta-nexell/recipes-core/images/<somefiles> \033[0m"
    echo -e "\033[44;33m You want revert, just below command >>\033[0m"
    echo -e "\033[44;33m python ../meta-nexell/tools/recipes-core-filename-change.py revert \033[0m"
    if [ "${IMAGE_TYPE}" == "tiny" -o "${IMAGE_TYPE}" == "qt" -o "${IMAGE_TYPE}" == "sato" ]
    then
	${PARENT_DIR}/meta-nexell/tools/recipes-core-filename-change.py ${IMAGE_TYPE}
    else
	usage
    fi
}

check_usage
make_result_dir
customize_conf_files
customize_recipe_core_files
