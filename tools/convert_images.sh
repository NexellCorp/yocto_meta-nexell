#!/bin/bash

set -e

result_dir=`pwd`
argc=$#
PARENT_DIR="${PWD%/*}"
ROOTDIR="root"
BOOTDIR="boot"
META_NEXELL_TOOLS_PATH="${PARENT_DIR}/meta-nexell/tools"

SECURE_TOOL=${META_NEXELL_TOOLS_PATH}/secure_tools/SECURE_BINGEN
RSA_SIGN_TOOL=${META_NEXELL_TOOLS_PATH}/secure_tools/rsa_sign_pss

IMAGE_TYPE=$2
MEM_SIZE=1024
MACHINE_NAME=$1
BOARD_SOCNAME=
BOARD_NAME=
BOARD_PREFIX=
BOARD_POSTFIX=

ARM_ARCH=

DEVID_USB=0
DEVID_SPI=1
DEVID_NAND=2
DEVID_SDMMC=3
DEVID_SDFS=4
DEVID_UART=5
PORT_EMMC=2
PORT_SD=0
DEVIDS=("usb" "spi" "nand" "sdmmc" "sdfs" "uart")
PORTS=("emmc" "sd")

# aes key
AES_KEY=
PRIVATE_KEY=${META_NEXELL_TOOLS_PATH}/secure_tools/private_key.pem
FIP_SEC_SIZE=
FIP_NONSEC_SIZE=

SECURE_BOOT="false"

MODULES_PATITION_SIZE=33554432

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
    echo "    ex) $0 s5p4418-navi-ref tinyui"
    echo "    ex) $0 s5p4418-cluster-ref qt"
    echo "    ex) $0 s5p4418-cluster-ref tiny"
    echo "    ex) $0 s5p4418-cluster-ref tinyui"
}

function convert_env_setup()
{
    echo "================================================="
    echo "convert_env_setup"
    echo "================================================="

    BOARD_SOCNAME=${MACHINE_NAME%-*-*}
    BOARD_NAME=${MACHINE_NAME#*-}
    BOARD_PREFIX=${BOARD_NAME%-*}
    BOARD_POSTFIX=${BOARD_NAME#*-}

    if [ ${BOARD_SOCNAME} == 's5p6818' ]; then
	ARM_ARCH="arm64"
    else
	ARM_ARCH="arm"
    fi
    
    if [ -a secure.cfg ]; then
        exec < secure.cfg
        read KEY VAL
        if [ $KEY == "SECURE" -a $VAL == "ON" ]; then
            SECURE_BOOT="true"
        else
	    SECURE_BOOT="false"
        fi
    else
	SECURE_BOOT="false"
    fi
}

function make_dirs()
{
    echo "================================================="
    echo "build make_dirs"
    echo "================================================="

    rm -rf ${BOOTDIR}
    rm -rf ${ROOTDIR}
    mkdir -m 777 ${BOOTDIR}
    mkdir -m 777 ${ROOTDIR}
}

function find_loop_device()
{
	local loop_dev=
	loop_dev=$(sudo losetup -a | grep ${ROOTDIR}.img | awk '{print $1}') && echo ${loop_dev%:*}
}

function mkramdisk()
{
    echo "================================================="
    echo "build mkramdisk"
    echo "================================================="
    
    if [ ${IMAGE_TYPE} != "tiny" ]; then	
            cp -a uInitrd ./boot
    else
	cp -a *.tar.bz2 ${ROOTDIR}
	cd ${ROOTDIR}
	tar -xvf *.tar.bz2
	rm *.tar.bz2
	rm -rf ./boot  # mean that {rootfs_dir}/boot
	cd ..

        pushd ${ROOTDIR}
        find . | cpio -o -H newc | gzip > ${result_dir}/initrd.gz
        popd

        ${META_NEXELL_TOOLS_PATH}/mkimage -A ${ARM_ARCH} -O linux -T ramdisk \
			             -C none -a 0 -e 0 -n uInitrd -d ${result_dir}/initrd.gz \
	                             ${result_dir}/boot/uInitrd
        rm -f ${result_dir}/initrd.gz	    
    fi
}

function mkparams()
{
    ${META_NEXELL_TOOLS_PATH}/mkenvimage -s 16384 -o params.bin default_envs.txt
}

function mkbootimg()
{
    echo "================================================="
    echo "mkbootimg"
    echo "================================================="

    if [ "${BOARD_SOCNAME}" == "s5p6818" ]; then
        cp -a Image ./boot
    elif [ "${BOARD_SOCNAME}" == "s5p4418" ]; then
        cp -a zImage ./boot
    else
        echo -e "Not supported Yet!!"
    fi
    
    cp -a *.dtb ./boot
    ${META_NEXELL_TOOLS_PATH}/make_ext4fs -b 4096 -L boot -l 33554432 boot.img ./boot/    
}

function make_2ndboot_for_emmc()
{
    echo "================================================="
    echo "make_2ndboot_for_emmc"
    echo "================================================="
    
    local bl1_source=
    local file_name=
    local chip_name=${BOARD_SOCNAME}
    local gen_img=
    local aes_out_img=
    local bootbingen=BOOT_BINGEN
    local aes_key=${1}

    if [ "${chip_name}" == "s5p6818" ]; then
	if [ ${SECURE_BOOT} == "true" ]; then
	    aes_key=${1}
	    echo -e "Yocto build not support aes encrypt! Something wrong!"
	    exit 1
	fi	    
	gen_img=bl1-emmcboot.bin.gen
	aes_in_img=${gen_img}
	aes_out_img=bl1-emmcboot.img
    elif [ "${chip_name}" == "s5p4418" ]; then
	gen_img=bl1-emmcboot.bin
    else
	gen_img=bl1-emmcboot.img
    fi
	
    # BINGEN
    if [ "${BOARD_NAME}" == "artik710-raptor" ]; then
        bl1_source=bl1-raptor.bin
        file_name=raptor-emmc-32.txt
    else
        bl1_source=bl1-${BOARD_PREFIX}.bin
        file_name=nsih_${BOARD_PREFIX}_ref_emmc.txt
    fi

    local nsih=${PARENT_DIR}/meta-nexell/tools/${MACHINE_NAME}/${file_name}

    if [ "${MACHINE_NAME}" == "s5p4418-navi-ref" ]; then
        chip_name="nxp4330"
        bootbingen=BOOT_BINGEN_NAVI
	elif [ "${MACHINE_NAME}" == "s5p4418-cluster-ref" ]; then
        chip_name="nxp4330"
        bootbingen=BOOT_BINGEN_NAVI
    fi

    ${PARENT_DIR}/meta-nexell/tools/${bootbingen} -c ${chip_name} -t 2ndboot -n ${nsih} -i ${bl1_source} -o ${gen_img} -l 0xffff0000 -e 0xffff0000

    # SECURE
    if [ "${chip_name}" == "s5p6818" ]; then
	if [ ${SECURE_BOOT} == "true" ]; then
            gen_hash_rsa ${gen_img} "" ${PRIVATE_KEY}
            dd if=${gen_img}.pub of=${gen_img} ibs=256 count=1 obs=512 seek=1 conv=notrunc
	    # AES encrypt
	    echo "SECURE BOOT not support Yocto build.\nIf you need to secure build, please contact us."
	    exit 1
	    aes_encrypt ${aes_out_img} ${aes_in_img} ${aes_key}
	else
	    cp ${gen_img} ${aes_out_img}
       fi
    fi
}

function make_3rdboot_for_emmc()
{
    echo "================================================="
    echo "make_3rdboot_for_emmc"
    echo "================================================="
    
    local file_name=
    local inout_image=u-boot
    local chip_name=${BOARD_SOCNAME}

    if [ "${BOARD_NAME}" == "artik710-raptor" ]; then
        file_name=raptor-emmc-32.txt
    else
        file_name=nsih_${BOARD_PREFIX}_ref_emmc.txt
    fi
    
    local nsih=${PARENT_DIR}/meta-nexell/tools/${MACHINE_NAME}/${file_name}
 
    local load_addr=
    local jump_addr=
    if [ "${BOARD_NAME}" == "s5p6818" ]; then
        case "${MEM_SIZE}" in
            512)  load_addr=0x5fc00000; jump_addr=0x5fe00000 ;;
            1024) load_addr=0x7fc00000; jump_addr=0x7fe00000 ;;
            2048) load_addr=0xbfc00000; jump_addr=0xbfe00000 ;;
        esac
    else
        load_addr=0x43c00000
        jump_addr=0x43c00000
    fi

    if [ "${MACHINE_NAME}" == "s5p4418-navi-ref" ]; then
        chip_name="nxp4330"
    elif [ "${MACHINE_NAME}" == "s5p4418-cluster-ref" ]; then
        chip_name="nxp4330"
    fi

    ${PARENT_DIR}/meta-nexell/tools/BOOT_BINGEN -c ${chip_name} -t 3rdboot -n ${nsih} -i ${inout_image}.bin -o singleimage-emmcboot.bin -l ${load_addr} -e ${jump_addr}
}

function post_process()
{
    echo "================================================="
    echo "post_process"
    echo "================================================="

    if [ "${BOARD_SOCNAME}" == "s5p6818" ]; then
	local aes_key=
        if [ ${SECURE_BOOT} == "true" ]; then
	    echo -e "Yocto build not support secure boot, something wrong!"
	    exit 1
	fi

        #make_2ndboot_for_sd ${result_dir} ${aes_key}	
	make_2ndboot_for_emmc ${aes_key}
	make_3rdboot_for_emmc
	
        gen_loader_emmc ${result_dir} fip-loader.bin ${PRIVATE_KEY} ${aes_key}
        gen_loader_sd ${result_dir} fip-loader.bin ${PRIVATE_KEY} ${aes_key}
        gen_secure ${result_dir} fip-secure.bin ${PRIVATE_KEY}
        gen_nonsecure ${result_dir} fip-nonsecure.bin ${PRIVATE_KEY}
	gen_loader_usb ${result_dir} fip-loader.bin ${PRIVATE_KEY} ${aes_key}
    else
	make_2ndboot_for_emmc
        make_3rdboot_for_emmc
    fi
    
    echo -e "\n\n\033[0;33m  Target download method.....                                                                \033[0m\n"
    if  [ "${MACHINE_NAME}" == "s5p6818-artik710-raptor" ]; then
        echo -e "\033[0;33m  <Full download>                                                                            \033[0m"
        echo -e "\033[0;33m      ex) $ ../meta-nexell/tools/update_s5p6818_artik710-raptor.sh -p partmap_emmc.txt -r .  \033[0m"
        echo -e "\033[0;33m  <kernel only>                                                                              \033[0m"
        echo -e "\033[0;33m      ex) $ ../meta-nexell/tools/update_s5p6818_artik710-raptor_kernel_only.sh               \033[0m"
        echo -e "\033[0;33m  <rootfs.img>                                                                               \033[0m"
        echo -e "\033[0;33m      ex) $ sudo fastboot flash rootfs rootfs.img                                            \033[0m\n"
    else
        echo -e "\033[0;33m  <Full download>                                                            \033[0m"
        echo -e "\033[0;33m      ex) $ ../meta-nexell/tools/update_s5p4418.sh -p partmap_emmc.txt -r .  \033[0m"
        echo -e "\033[0;33m  <bl1, u-boot, kernel only>                                                 \033[0m"
        echo -e "\033[0;33m      ex) $ ../meta-nexell/tools/update_s5p4418_kernel_uboot_bl1_only.sh     \033[0m"
        echo -e "\033[0;33m  <rootfs.img>                                                               \033[0m"
        echo -e "\033[0;33m      ex) $ sudo fastboot flash rootfs rootfs.img                            \033[0m\n"
    fi
}

#1-result dir 2-in_img 3-private_key 4-aes_key
function gen_loader_emmc() {
    gen_loader ${1} ${2} ${3} ${DEVID_SDMMC} ${PORT_EMMC} ${4}
    
}

function gen_loader_sd() {
    gen_loader ${1} ${2} ${3} ${DEVID_SDMMC} ${PORT_SD} ${4}
}

function gen_loader_usb() {
    gen_loader ${1} ${2} ${3} ${DEVID_USB} 0 ${4}

    pushd ${1}
    cat fip-secure.img >> fip-loader-usb.img
    cat fip-nonsecure.img >> fip-loader-usb.img
    popd
}

# args
# 1; result_dir
# 2: input fip-loader.bin
# 3: rsa key
# 4: boot device number
# 5: port
# 6: aes key
function gen_loader() {
    local result_dir=${1}
    local chip_name=${BOARD_SOCNAME}

    local in_img=${2}
    local out_img=
    local gen_img="${in_img}".gen
    local aes_in_img="${gen_img}"
    local aes_out_img=
    local devname=${5}

    local hash_name="${in_img}".hash
    local private_key=${3}
    local aes_key=${6}

    local load_addr=0x7fcc0000 #0x7fdce000
    local jump_addr=0x7fd00800 #0x7fe00800
    local bootdev=${4}
    local portnum=${5}

    local bl1_source=
    local file_name=

    if [ "${chip_name}" == "s5p6818" ]; then
        if [ "${BOARD_NAME}" == "artik710-raptor" ]; then
	    file_name=raptor-64-secure.txt
	elif [ "${BOARD_NAME}" == "avn-ref" ]; then
	    file_name=avn-64-secure.txt
	fi
    else
	echo "error! is not s5p6818"
	#temporary
	exit 1
#	bl1_source=bl1-artik530
#	file_name=${BOARD_PURENAME}-sd.txt
    fi

    local nsih=${PARENT_DIR}/meta-nexell/tools/${MACHINE_NAME}/${file_name}

    if [ ! -f ${result_dir}/${in_img} ]; then
	echo "${in_img} not found!"
	exit 1
    fi

    if [ ! -f ${private_key} ]; then
	echo " private key '${private_key}' not found!"
	exit 1
    fi

    if [ -z ${load_addr} ] || [ -z ${jump_addr} ]; then
	echo " Enter load/jump address."
	exit 1
    fi

    # parsing bootdev, portnum
    if [ -z ${bootdev} ]; then
	bootdev=${DEVID_SDMMC}
    fi

    if [ -z ${portnum} ]; then
	portnum=${PORT_EMMC}
    fi

    devname=${DEVIDS[${bootdev}]}
    if [ ${bootdev} == ${DEVID_SDMMC} ]; then
	if [ ${portnum} == ${PORT_EMMC} ]; then
	    devname="emmc"
	else
	    devname="sd"
	fi

	dev_offset_opts="-m 0x60200 -b ${bootdev} -p ${portnum} \
		-m 0x1E0200 -b ${bootdev} -p ${portnum}"
    elif [ ${bootdev} == ${DEVID_USB} ]; then
	dev_offset_opts="-u -m 0x7fb00000 -z ${FIP_SEC_SIZE} \
		-m 0x7df00000 -z ${FIP_NONSEC_SIZE}"
    fi
    out_img="fip-loader-${devname}.img"
    aes_out_img="${out_img}"
    echo "[fip-loader] bootdev: ${bootdev}"
    echo "[fip-loader] out_img: ${out_img}"

    # BINGEN
    ${SECURE_TOOL} -c ${chip_name} -t 3rdboot -n ${nsih} \
		   -i ${result_dir}/${in_img} \
		   -o ${result_dir}/${gen_img} \
		   -l ${load_addr} -e ${jump_addr} \
		   -k ${bootdev} \
		   ${dev_offset_opts}

    if [ ${SECURE_BOOT} == "true" ]; then	
	echo "Yocto build does not support SECURE boot"
	break
	# AES encrypt
	aes_encrypt ${aes_out_img} ${aes_in_img} ${aes_key}
    fi
    
    cp ${gen_img} ${aes_out_img}
}

# fip-secure.bin
function gen_secure() {
    local result_dir=${1}
    local chip_name=${BOARD_SOCNAME}

    local in_img=${2}
    local out_img=fip-secure.img
    local gen_img="${in_img}".gen

    local hash_name="${in_img}".hash
    local private_key=${3}

    local load_addr=0x7fb00000 #0x7fd00000
    local jump_addr=0x00000000

    local bl1_source=
    local file_name=
    if [ "${chip_name}" == "s5p6818" ]; then
	if [ "${BOARD_NAME}" == "artik710-raptor" ]; then
	    file_name=${BOARD_POSTFIX}-64.txt
	elif [ "${BOARD_NAME}" == "avn-ref" ]; then
	    file_name=avn-64.txt
	fi
    else
	echo "error! is not s5p6818"
	#temporary
	exit 1
#	bl1_source=bl1-artik530
#	file_name=${BOARD_PURENAME}-sd.txt
    fi

    local nsih=${PARENT_DIR}/meta-nexell/tools/${MACHINE_NAME}/${file_name}
    
    if [ ! -f ${result_dir}/${in_img} ]; then
	echo "${in_img} not found!"
	exit 1
    fi

    if [ ! -f ${private_key} ]; then
	echo " private key '${private_key}' not found!"
	exit 1
    fi

    if [ -z ${load_addr} ] || [ -z ${jump_addr} ]; then
	echo " Enter load/jump address."
	exit 1
    fi

    # BINGEN
    ${SECURE_TOOL} -c ${chip_name} -t 3rdboot -n ${nsih} \
		   -i ${result_dir}/${in_img} \
		   -o ${result_dir}/${gen_img} \
		   -l ${load_addr} -e ${jump_addr}

    if [ ${SECURE_BOOT} == "true" ]; then
	echo "Yocto build does not support SECURE boot"
	break	
        # RSA
        gen_hash_rsa ${in_img} ${hash_name} ${private_key}
        write_hash_rsa ${gen_img} /dev/null ${in_img}.sig
    fi
    
    cp ${gen_img} ${out_img}

    FIP_SEC_SIZE=`stat --printf="%s" ${out_img}`
}

# fip-nonsecure.bin
function gen_nonsecure() {
    local result_dir=${1}
    local chip_name=${BOARD_SOCNAME}

    local in_img=${2}
    local out_img=fip-nonsecure.img
    local gen_img="${in_img}".gen

    local hash_name="${in_img}".hash
    local private_key=${3}

    local load_addr=0x7df00000 #0x7fb00000
    local jump_addr=0x00000000

    local bl1_source=
    local file_name=
    if [ "${chip_name}" == "s5p6818" ]; then
	if [ "${BOARD_NAME}" == "artik710-raptor" ]; then
	    file_name=${BOARD_POSTFIX}-64.txt
	elif [ "${BOARD_NAME}" == "avn-ref" ]; then
	    file_name=avn-64.txt
	fi
    else
	echo "error! is not s5p6818"
	#temporary
	exit 1
#	bl1_source=bl1-artik530
#	file_name=${BOARD_PURENAME}-sd.txt
    fi

    local nsih=${PARENT_DIR}/meta-nexell/tools/${MACHINE_NAME}/${file_name}

    if [ ! -f ${result_dir}/${in_img} ]; then
	echo "${in_img} not found!"
	exit 1
    fi

    if [ ! -f ${private_key} ]; then
	echo " private key '${private_key}' not found!"
	exit 1
    fi

    if [ -z ${load_addr} ] || [ -z ${jump_addr} ]; then
	echo " Enter load/jump address."
	exit 1
    fi

    # BINGEN
    ${SECURE_TOOL} -c ${chip_name} -t 3rdboot -n ${nsih} \
		   -i ${result_dir}/${in_img} \
		   -o ${result_dir}/${gen_img} \
		   -l ${load_addr} -e ${jump_addr}

    if [ ${SECURE_BOOT} == "true" ]; then
	echo "Yocto build does not support SECURE boot"
        break
        # RSA
        gen_hash_rsa ${in_img} ${hash_name} ${private_key}
        write_hash_rsa ${gen_img} /dev/null ${in_img}.sig
    fi
    
    cp ${gen_img} ${out_img}

    FIP_NONSEC_SIZE=`stat --printf="%s" ${out_img}`
}

#write_hash_rsa ${gen_img} ${in_img}.pub ${in_img}.sig
function write_hash_rsa() {
    img=${1}
    pub=${2}
    sig=${3}

    dd if=${pub} of=${img} conv=notrunc ibs=256 count=1 obs=256 seek=2
    dd if=${sig} of=${img} conv=notrunc ibs=256 count=1 obs=256 seek=3
}

function aes_encrypt() {
    out_img=${1}
    in_img=${2}
    aes_key=${3}
    
    if [ ! -f ${aes_key} ]; then
	echo "${aes_key} not found!"
	exit 1
    fi

    openssl enc -aes-128-ecb -e \
	    -in ${in_img} \
	    -out ${out_img} -p -nosalt \
	    -K  `cat ${aes_key}`
}

# secure common
function gen_hash_rsa() {
    echo "================================================="
    echo "gen_hash_rsa"
    echo "================================================="
    echo "in_img = ${1}"
    echo "hash_name = ${2}"
    echo "private_key = ${3}"
    echo "================================================="
    local in_img=${1}
    local hash_name=${2}
    local private_key=${3}

    # generate hash ... skip
    #openssl dgst -sha256 -binary -out ${hash_name} ${in_img}

    # generate sig, pub
    #echo "private key: ${private_key}"
    #echo "src: ${in_img}"
    ${RSA_SIGN_TOOL} ${private_key} ${in_img}

    # <output>
    #     ${in_img}.sig
    #     ${in_img}.pub
}

#write_hash_rsa ${gen_img} ${in_img}.pub ${in_img}.sig
function write_hash_rsa() {
    img=${1}
    pub=${2}
    sig=${3}

    dd if=${pub} of=${img} conv=notrunc ibs=256 count=1 obs=256 seek=2
    dd if=${sig} of=${img} conv=notrunc ibs=256 count=1 obs=256 seek=3
}

function make_modules() {
    mkdir -p modules
    cp -a modules*.tgz modules
    cd modules
    tar -xvzf modules*.tgz
    cd ..
    ${META_NEXELL_TOOLS_PATH}/make_ext4fs -b 4096 -L modules -l ${MODULES_PATITION_SIZE} modules.img modules
    
}

check_usage
convert_env_setup
make_dirs
mkramdisk
mkparams
mkbootimg
post_process
#make_modules
