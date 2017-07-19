#!/bin/bash
#Description : This script only use s5p6818 for optee build
argc=$#
MACHINE_NAME=$1
BOARD_SOCNAME=${MACHINE_NAME%-*-*}

function check_usage()
{
    if [ $argc -lt 1 ]
    then
	echo "Invalid argument check usage please"
	usage
	exit
    fi
}

function parse_args()
{
    ARGS=$(getopt -o h -- "$@");
    eval set -- "$ARGS";

    while true; do
            case "$1" in
                -h ) usage; exit 1 ;;
                -- ) break ;;
            esac
    done
}

function usage()
{
    echo "It must run in the 'build' directory."
    echo "Usage: $0 <machine-name>"
    echo "    ex) build-s5p6818-artik710-raptor$ $0 s5p6818-artik710-raptor"
    echo "    ex) build-s5p6818-avn-ref$ $0 s5p6818-avn-ref "
}

function pre_operation()
{
    if [ ${BOARD_SOCNAME} != "s5p6818" ];then
	echo -e "\033[0;33m s5p4418 board not use optee & ATF \033[0m"
    else
        echo -e "\n\033[47;34m ------------------------------------------------------------------ \033[0m"
        echo -e "\033[47;34m  ATF & l-loader & optee & u-boot & bl1 ==> do_patch                \033[0m"
        echo -e "\033[47;34m ------------------------------------------------------------------ \033[0m"
        bitbake arm-trusted-firmware l-loader optee-client optee-os optee-test optee-linuxdriver bl1-${BOARD_SOCNAME} u-boot-nexell -c patch

        echo -e "\033[47;34m ------------------------------------------------------------------ \033[0m"
        echo -e "\033[47;34m                        do_patch Done...                            \033[0m"
        echo -e "\033[47;34m ------------------------------------------------------------------ \033[0m"
    fi
}

parse_args $@
check_usage
pre_operation
