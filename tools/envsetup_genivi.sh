#!/bin/bash

#genivi_setup.sh location --> meta-nexell/tools/genivi_setup.sh
CURRENT_PATH=`dirname $0`
TOOLS_PATH=`readlink -ev $CURRENT_PATH`

ROOT_PATH=

argc=$#
MACHINE_NAME=$1

BOARD_SOCNAME=
BOARD_NAME=
BOARD_PREFIX=
BOARD_POSTFIX=

GENIVI_PATH=
GENIVI_BUILD_PATH=
META_NEXELL_PATH=

QT5_7="true"

declare -a targets=("s5p4418-avn-ref" "s5p4418-navi-ref" "s5p6818-artik710-raptor" "s5p6818-avn-ref")

function check_usage()
{
    if [ $argc -lt 1 ]
    then
	echo "Invalid argument check usage please"
	usage
	exit
    fi	
}

function usage()
{
    echo "Usage: $0 <machine-name> <call-type>"
    echo "    ex) $0 s5pxxxx-xxx-xxx "
    echo "    ex) $0 s5pxxxx-xxx-xxx "
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
    ROOT_PATH=`readlink -ev ${TOOLS_PATH}/../..`
    META_NEXELL_PATH=`readlink -ev ${ROOT_PATH}/meta-nexell`
    GENIVI_PATH=`readlink -ev ${ROOT_PATH}/GENIVI`

    if ! [ -d ${ROOT_PATH}/build-${MACHINE_NAME}-genivi ]; then
        mkdir -p ${ROOT_PATH}/build-${MACHINE_NAME}-genivi
    fi

    GENIVI_BUILD_PATH=`readlink -ev ${ROOT_PATH}/build-${MACHINE_NAME}-genivi`
}

function customize_conf_files()
{
    cp -a ${GENIVI_PATH}/gdp-src-build/* ${GENIVI_BUILD_PATH}/

    #check exist files
    if ! [ -a ${META_NEXELL_PATH}/misc/GENIVI/init.sh ];then
	echo "init.sh not exist"
	exit
    elif ! [ -a ${META_NEXELL_PATH}/misc/GENIVI/${BOARD_SOCNAME}-sample.local.inc ];then
	echo "${BOARD_SOCNAME}-sample.local.inc not exist"
	exit
    elif ! [ -a ${META_NEXELL_PATH}/misc/GENIVI/nexell.bblayers.conf ];then
	echo "nexell.bblayers.conf not exist"
	exit
    fi

    #bblayers.inc overwrite
    cp -a ${META_NEXELL_PATH}/misc/GENIVI/bblayers.inc ${GENIVI_BUILD_PATH}/conf/templates/

    #init.sh overwrite
    cp -a ${META_NEXELL_PATH}/misc/GENIVI/init.sh ${GENIVI_PATH}

    #*.local.conf
    echo "include templates/${MACHINE_NAME}.local.inc" > ${GENIVI_BUILD_PATH}/conf/templates/nexell.local.conf
    echo "MACHINE = \"${MACHINE_NAME}\"" >> ${GENIVI_BUILD_PATH}/conf/templates/nexell.local.conf
    echo "ALLOW_EMPTY_${MACHINE_NAME}-bl1 = \"1\"" >> ${GENIVI_BUILD_PATH}/conf/templates/nexell.local.conf
    echo "ALLOW_EMPTY_${MACHINE_NAME}-uboot = \"1\"" >> ${GENIVI_BUILD_PATH}/conf/templates/nexell.local.conf
    #cp -a ${GENIVI_BUILD_PATH}/conf/templates/nexell.local.conf ${GENIVI_BUILD_PATH}/conf/local.conf

    #*.bblayers.conf
    cp -a ${META_NEXELL_PATH}/misc/GENIVI/nexell.bblayers.conf ${GENIVI_BUILD_PATH}/conf/templates/nexell.bblayers.conf
    echo "BBLAYERS += \" \\" >> ${GENIVI_BUILD_PATH}/conf/templates/nexell.bblayers.conf
    echo "    ${META_NEXELL_PATH} \\" >> ${GENIVI_BUILD_PATH}/conf/templates/nexell.bblayers.conf
    echo "    \" " >> ${GENIVI_BUILD_PATH}/conf/templates/nexell.bblayers.conf
    #cp -a ${GENIVI_BUILD_PATH}/conf/templates/${MACHINE_NAME}.bblayers.conf ${GENIVI_BUILD_PATH}/conf/bblayers.conf

    #*.local.inc
    cp -a ${META_NEXELL_PATH}/misc/GENIVI/nexell-common.local.inc ${GENIVI_BUILD_PATH}/conf/templates/nexell-common.local.inc
    cp -a ${META_NEXELL_PATH}/misc/GENIVI/${BOARD_SOCNAME}-sample.local.inc ${GENIVI_BUILD_PATH}/conf/templates/${MACHINE_NAME}.local.inc
    genivi_bbmasking
}

function genivi_bbmasking()
{
    for i in ${targets[@]}
    do
        echo "BBMASK += \" /meta-nexell/recipes-core/images/$i\"" >> ${GENIVI_BUILD_PATH}/conf/templates/${MACHINE_NAME}.local.inc
    done

    sed -i "/\/meta-nexell\/recipes-core\/images\/${MACHINE_NAME}/d" ${GENIVI_BUILD_PATH}/conf/templates/${MACHINE_NAME}.local.inc

    if [ ${QT5_7} == "true" ];then
	echo "BBMASK += \" /meta-genivi-dev/meta-genivi-dev/recipes-qt/qt5/qtwayland_%.bbappend\"" >> ${GENIVI_BUILD_PATH}/conf/templates/${MACHINE_NAME}.local.inc
    fi
}

function genivi_patch_applied()
{
    if [ ${QT5_7} == "true" ];then
        #qtwayland, QT5.7 patch apply
        cd ${META_NEXELL_PATH}/recipes-qt/qt5
        mkdir -p qtwayland

        cp -a ${META_NEXELL_PATH}/misc/GENIVI/qtwayland/0001-qtwayland_nexell_5.7.patch ${META_NEXELL_PATH}/recipes-qt/qt5/qtwayland/.
        cp -a ${META_NEXELL_PATH}/misc/GENIVI/qtwayland/qtwayland_%.bbappend ${META_NEXELL_PATH}/recipes-qt/qt5/.
    else
        if [ -d ${META_NEXELL_PATH}/recipes-qt/qt5/qtwayland ];then
	    rm -rf ${META_NEXELL_PATH}/recipes-qt/qt5/qtwayland
	fi

	if [ -a ${META_NEXELL_PATH}/recipes-qt/qt5/qtwayland_%.bbappend ];then
	    rm -rf ${META_NEXELL_PATH}/recipes-qt/qt5/qtwayland_%.bbappend
	fi
    fi
}

function copy_build_scripts()
{
    local secure=
    local TMP_WORK_PATH=${GENIVI_BUILD_PATH}/tmp/work

    if ! [ -d ${TMP_WORK_PATH} ];then
	mkdir -p ${TMP_WORK_PATH}
    fi
    
    #temp ARM 32bit build toolchain copy
    echo -e "\033[0;33m #########  Start toolchain copy to tmp/work/ ########## \033[0m"
    ${META_NEXELL_PATH}/tools/toolchain/toolchain_setup.sh ${META_NEXELL_PATH} ${TMP_WORK_PATH}

    #for secure boot support
    if [ "${BOARD_SOCNAME}" == "s5p6818" ]; then
	echo "SECURE OFF" > ${GENIVI_BUILD_PATH}/secure.cfg; secure="OFF"
	python ${META_NEXELL_PATH}/tools/secure_tools/secure-setup.py ${secure} ${MACHINE_NAME} ${META_NEXELL_PATH}
    fi

    touch ${TMP_WORK_PATH}/source_dir_path.txt
    touch ${TMP_WORK_PATH}/source_kernel_dir_path.txt

    cp -a ${META_NEXELL_PATH}/tools/bitbake_pre_operation.sh ${GENIVI_BUILD_PATH}
    echo -e "\033[0;33m                                                                    \033[0m"
    echo -e "\033[0;33m #########  Start bitbake pre operateion for optee & ATF ########## \033[0m"
    echo -e "\033[0;33m                                                                    \033[0m"

    #-----------------------------------------------
    # bitbake pre-tasks process related optee & atf
    #-----------------------------------------------
    if [ ! -e ${GENIVI_BUILD_PATH}/OPTEE_PRE_OPERATION_DONE ];then
	${GENIVI_BUILD_PATH}/bitbake_pre_operation.sh ${MACHINE_NAME}
	touch ${GENIVI_BUILD_PATH}/OPTEE_PRE_OPERATION_DONE
    else
	echo -e "\033[0;33m #########  Already Done, optee & ATF pre-fetch & pre-unpack ########## \033[0m\n"
    fi

    echo -e "\n"
    echo -e "\033[0;33m #########  You are select GENIVI platform build with ${MACHINE_NAME}  ########## \033[0m"
    echo -e "\033[0;33m Next step : source init.sh nexell ${MACHINE_NAME} \033[0m\n"
    echo -e "\033[0;33m ################################################################################ \033[0m"
}

check_usage
split_args
path_setup
customize_conf_files
genivi_patch_applied
copy_build_scripts
