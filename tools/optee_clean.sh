#!/bin/bash

argc=$#
MACHINE_NAME=$1

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
    echo "Only s5p6818 soc, It must run in the 'build' directory."
    echo "Usage: $0 <machine-name> "
    echo "    ex) build-s5p6818-artik710-raptor$ $0 s5p6818-artik710-raptor "
    echo "    ex) build-s5p6818-avn-ref$ $0 s5p6818-avn-ref "
}

function optee_clean()
{
    rm -rf OPTEE_PRE_OPERATION_DONE

    bitbake -c cleansstate arm-trusted-firmware l-loader optee-build optee-client optee-os optee-test optee-linuxdriver ${MACHINE_NAME}-bl1 ${MACHINE_NAME}-uboot
}

parse_args $@
check_usage
optee_clean
