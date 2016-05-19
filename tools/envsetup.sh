#!/bin/bash

set -e

argc=$#
BOARD_NAME=$1
RESULT_DIR="result-$1"
PARENT_DIR="${PWD%/*}"
RESULT_PATH="${PARENT_DIR}/${RESULT_DIR}"

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
    echo "Usage: $0 <board-name>"
    echo "    ex) $0 artik710-raptor"
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
    ${PARENT_DIR}/meta-nexell/tools/setup-conf-files.py ${BOARD_NAME}
    echo ${RESULT_PATH} > result_path.txt
}
check_usage
make_result_dir
customize_conf_files
