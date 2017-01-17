#!/bin/bash

set -e

CURRENT_PATH=`dirname $0`
TOOLS_PATH=`readlink -ev $CURRENT_PATH`

argc=$#
RESULT_DIR="result-$1-$2"
PARENT_DIR="${PWD%/*}"
RESULT_PATH="${PARENT_DIR}/${RESULT_DIR}"
IMAGE_TYPE=$2
SDK_RELEASE=$3
NEXELL_CODE_MASK="NEXELL appended code"

MACHINE_NAME=$1
BOARD_SOCNAME=
BOARD_NAME=
BOARD_PREFIX=
BOARD_POSTFIX=

META_NEXELL_PATH=
NEXELL_BUILD_PATH=./

declare -a targets=("s5p4418-avn-ref" "s5p4418-navi-ref" "s5p6818-artik710-raptor" "s5p6818-avn-ref")

function check_usage()
{
    if [ ${IMAGE_TYPE} == "genivi" ]; then
	echo "Invalid argument image type!!"
	echo "This script does not support GENIVI"
	echo "You must be using envsetup_genivi.sh"
        exit
    fi
   
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
    echo "Usage: $0 <machine-name> <image-type> <sdk:true or false>"
    echo "    ex) $0 s5p6818-artik710-raptor tiny"
    echo "    ex) $0 s5p6818-artik710-raptor sato"
    echo "    ex) $0 s5p6818-artik710-raptor qt"
    echo "    ex) $0 s5p4418-avn-ref qt"
    echo "    ex) $0 s5p4418-avn-ref tiny"
    echo "    ex) $0 s5p4418-navi-ref qt"
    echo "    ex) $0 s5p4418-navi-ref sato"
    echo "    ex) $0 s5p4418-navi-ref tiny"
    echo "    ex) $0 s5p4418-navi-ref tinyui"
    echo "    ex) $0 s5p4418-navi-ref qt sdk"
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
    META_NEXELL_PATH=`readlink -ev ${TOOLS_PATH}/..`
    if ! [ -d ${META_NEXELL_PATH}/../build-${MACHINE_NAME}-${IMAGE_TYPE} ]; then
	echo "Warning, please check -  source poky/oe-init-build-env build-<machine_name>-<image_type>"
        exit
    else
	NEXELL_BUILD_PATH=`readlink -ev ${META_NEXELL_PATH}/../build-${MACHINE_NAME}-${IMAGE_TYPE}`
    fi
}

function customize_conf_files()
{
    #genivi overrided files remove
    if [ -d ${META_NEXELL_PATH}/recipes-qt/qt5/qtwayland ];then
	rm -rf ${META_NEXELL_PATH}/recipes-qt/qt5/qtwayland
	rm -rf ${META_NEXELL_PATH}/recipes-qt/qt5/qtwayland_%.bbappend
    fi

    cp ${META_NEXELL_PATH}/misc/local.conf.org ${NEXELL_BUILD_PATH}/conf/local.conf
    cp ${META_NEXELL_PATH}/misc/bblayers-${IMAGE_TYPE}-sample.~conf ${NEXELL_BUILD_PATH}/conf/bblayers.conf

    #bbmask & PREFERRED_PROVIDER append
    local_conf_append

    echo ${RESULT_PATH} > result_path.txt
}

function local_conf_append()
{
    #local conf bbmasking append
    echo "#NEXELL appended code" >> ${NEXELL_BUILD_PATH}/conf/local.conf

    for i in ${targets[@]}
    do
	echo "BBMASK += \" /meta-nexell/recipes-core/images/$i\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
    done

    sed -i "/\/meta-nexell\/recipes-core\/images\/${MACHINE_NAME}/d" ${NEXELL_BUILD_PATH}/conf/local.conf

    ${META_NEXELL_PATH}/tools/setup-conf-files.py ${NEXELL_BUILD_PATH} ${MACHINE_NAME} ${IMAGE_TYPE} ${META_NEXELL_PATH}

    #genivi override file remove
    echo "BBMASK += \" /meta-nexell/recipes-core/packagegroups/packagegroup-gdp-qt5.bbappend\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
    echo "BBMASK += \" /meta-nexell/recipes-dev-platform/images/genivi-dev-platform.bbappend\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf

    #dependancy MACHINE NAME so setup-conf_file.py can't included
    if [ ${IMAGE_TYPE} != "tiny" ]; then
        local imgtype=
        if [ ${IMAGE_TYPE} == "sato" ]; then
            imgtype="-sato"
        fi
        echo "PREFERRED_PROVIDER_virtual/libgles1 = \"nexell-drm-mali${imgtype}\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
        echo "PREFERRED_PROVIDER_virtual/libgles2 = \"nexell-drm-mali${imgtype}\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
        echo "PREFERRED_PROVIDER_virtual/egl = \"nexell-drm-mali${imgtype}\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
        echo "PREFERRED_PROVIDER_virtual/libopencl = \"nexell-drm-mali${imgtype}\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
        echo "PREFERRED_PROVIDER_virtual/libgl = \"mesa\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
        echo "PREFERRED_PROVIDER_virtual/mesa = \"mesa\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
        echo "PREFERRED_PROVIDER_libgbm = \"nexell-drm-mali${imgtype}\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
        echo "PREFERRED_PROVIDER_libgbm-dev = \"nexell-drm-mali${imgtype}\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
    else
        if [ ${BOARD_SOCNAME} == "s5p6818" ]; then
            echo "SERIAL_CONSOLE = \"115200 ttySAC3\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
        else
            echo "SERIAL_CONSOLE = \"115200 ttyAMA3\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
        fi
        echo "USE_VT = \"1\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
    fi

    #sdk related code clean for rebuild time
    if [ ${SDK_RELEASE} == "false" ]; then
        echo "BBMASK += \" /meta-nexell/recipes-core/images/meta-environment.bbappend\"" >> ${NEXELL_BUILD_PATH}/conf/local.conf
    fi
}

function customize_recipe_core_files()
{
    ${META_NEXELL_PATH}/tools/recipes-core-filename-change.py ${META_NEXELL_PATH} ${MACHINE_NAME} ${IMAGE_TYPE}
}

function copy_build_scripts()
{
    local secure=
    local TMP_WORK_PATH=${NEXELL_BUILD_PATH}/tmp/work

    if ! [ -d $TMP_WORK_PATH ];then
	mkdir -p $TMP_WORK_PATH
    fi
    
    #temp ARM 32bit build toolchain copy
    echo -e "\033[0;33m #########  Start toolchain copy to tmp/work/ ########## \033[0m"
    ${META_NEXELL_PATH}/tools/toolchain/toolchain_setup.sh ${META_NEXELL_PATH} ${TMP_WORK_PATH}

#    #for secure boot support
    if [ "${BOARD_SOCNAME}" == "s5p6818" ]; then	
#	while :
#	do
#	    if [ "$ERR_MSG" != "" ]; then
#		echo "Error: $ERR_MSG"
#		echo ""
#	    fi

#	    echo -e "\033[40;33m Artik710-raptor board support secure boot, Please select build option  \033[0m"
#	    echo -e "\033[40;33m     1:       Secure build  \033[0m"
#	    echo -e "\033[40;33m     2:   Non-Secure build  \033[0m"
#	    ERR_MSG=""

#	    read -p " Select : " SEL
#	    case $SEL in
#		1) echo "SECURE ON" > secure.cfg; secure="ON"; break ;;
#		2) echo "SECURE OFF" > secure.cfg; secure="OFF"; break ;;
#		*) ERR_MSG="Please enter a valid option!"
#	    esac
#   	done
	# default secure off
        echo "SECURE OFF" > ${NEXELL_BUILD_PATH}/secure.cfg; secure="OFF"
        python ${META_NEXELL_PATH}/tools/secure_tools/secure-setup.py ${secure} ${MACHINE_NAME} ${META_NEXELL_PATH}

    fi

    touch ${TMP_WORK_PATH}/source_dir_path.txt
    touch ${TMP_WORK_PATH}/source_kernel_dir_path.txt

    cp -a ${META_NEXELL_PATH}/tools/bitbake_pre_operation.sh ${NEXELL_BUILD_PATH}
    echo -e "\033[0;33m                                                                    \033[0m"
    echo -e "\033[0;33m #########  Start bitbake pre operateion for optee & ATF ########## \033[0m"
    echo -e "\033[0;33m                                                                    \033[0m"

    #-----------------------------------------------
    # bitbake pre-tasks process related optee & atf
    #-----------------------------------------------
    if [ ! -e ${NEXELL_BUILD_PATH}/OPTEE_PRE_OPERATION_DONE ];then
	${NEXELL_BUILD_PATH}/bitbake_pre_operation.sh ${MACHINE_NAME}
	touch ${NEXELL_BUILD_PATH}/OPTEE_PRE_OPERATION_DONE
    else
	echo -e "\033[0;33m #########  Already Done, optee & ATF pre-fetch & pre-unpack ########## \033[0m"
    fi
}
    
check_usage
split_args
path_setup
customize_conf_files
copy_build_scripts
