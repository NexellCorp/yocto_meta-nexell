#!/bin/bash

set -e

argc=$#
CURRENT_PATH=`dirname $0`
ROOTDIR="root"
BOOTDIR="boot"
META_NEXELL_TOOLS_PATH=`readlink -ev ${CURRENT_PATH}/../`
META_NEXELL_CONVERT_TOOLS_PATH=`readlink -ev ${CURRENT_PATH}`
RESULT_NAME="result-${1}-${2}"
RESULT_PATH="${META_NEXELL_TOOLS_PATH}/../../../out/${RESULT_NAME}"
result_dir=`readlink -ev ${RESULT_PATH}`

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

MEM_256MB_LOAD_ADDR=0x4fcc0000
MEM_256MB_JUMP_ADDR=0x4fd00800
MEM_256MB_SECURE_LOAD_ADDR=0x4fb00000
MEM_256MB_SECURE_JUMP_ADDR=0x00000000
MEM_256MB_NON_SECURE_LOAD_ADDR=0x4df00000
MEM_256MB_NON_SECURE_JUMP_ADDR=0x00000000

MEM_512MB_LOAD_ADDR=0x9fcc0000
MEM_512MB_JUMP_ADDR=0x9fd00800
MEM_512MB_SECURE_LOAD_ADDR=0x9fb00000
MEM_512MB_SECURE_JUMP_ADDR=0x00000000
MEM_512MB_NON_SECURE_LOAD_ADDR=0x9df00000
MEM_512MB_NON_SECURE_JUMP_ADDR=0x00000000

MEM_1G_LOAD_ADDR=0x7fcc0000
MEM_1G_JUMP_ADDR=0x7fd00800
MEM_1G_SECURE_LOAD_ADDR=0x7fb00000
MEM_1G_SECURE_JUMP_ADDR=0x00000000
MEM_1G_NON_SECURE_LOAD_ADDR=0x7df00000
MEM_1G_NON_SECURE_JUMP_ADDR=0x00000000

MEM_2G_LOAD_ADDR=0xbfcc0000
MEM_2G_JUMP_ADDR=0xbfd00800
MEM_2G_SECURE_LOAD_ADDR=0xbfb00000
MEM_2G_SECURE_JUMP_ADDR=0x00000000
MEM_2G_NON_SECURE_LOAD_ADDR=0xbdf00000
MEM_2G_NON_SECURE_JUMP_ADDR=0x00000000

MEM_LOAD_ADDR=
MEM_JUMP_ADDR=
MEM_SECURE_LOAD_ADDR=
MEM_SECURE_JUMP_ADDR=
MEM_NON_SECURE_LOAD_ADDR=
MEM_NON_SECURE_JUMP_ADDR=

declare -a mem_256MB_addrs=( $MEM_256MB_LOAD_ADDR \
                             $MEM_256MB_JUMP_ADDR \
                             $MEM_256MB_SECURE_LOAD_ADDR \
                             $MEM_256MB_SECURE_JUMP_ADDR \
                             $MEM_256MB_NON_SECURE_LOAD_ADDR \
                             $MEM_256MB_NON_SECURE_JUMP_ADDR \
                           )
declare -a mem_512MB_addrs=( $MEM_512MB_LOAD_ADDR \
                             $MEM_512MB_JUMP_ADDR \
                             $MEM_512MB_SECURE_LOAD_ADDR \
                             $MEM_512MB_SECURE_JUMP_ADDR \
                             $MEM_512MB_NON_SECURE_LOAD_ADDR \
                             $MEM_512MB_NON_SECURE_JUMP_ADDR \
                           )
declare -a mem_1G_addrs=( $MEM_1G_LOAD_ADDR \
                          $MEM_1G_JUMP_ADDR \
                          $MEM_1G_SECURE_LOAD_ADDR \
                          $MEM_1G_SECURE_JUMP_ADDR \
                          $MEM_1G_NON_SECURE_LOAD_ADDR \
                          $MEM_1G_NON_SECURE_JUMP_ADDR \
                        )
declare -a mem_2G_addrs=( $MEM_2G_LOAD_ADDR \
                          $MEM_2G_JUMP_ADDR \
                          $MEM_2G_SECURE_LOAD_ADDR \
                          $MEM_2G_SECURE_JUMP_ADDR \
                          $MEM_2G_NON_SECURE_LOAD_ADDR \
                          $MEM_2G_NON_SECURE_JUMP_ADDR \
                        )

#------------------------------------
# dev_portnum define
declare -A targets_dev_portnum
targets_dev_portnum["avn-ref"]=2
targets_dev_portnum["navi-ref"]=0
targets_dev_portnum["bitminer-ref"]=0
targets_dev_portnum["zh-dragon"]=0
targets_dev_portnum["daudio-ref"]=0
targets_dev_portnum["daudio-covi"]=0
targets_dev_portnum["smart-voice"]=0
targets_dev_portnum["ff-voice"]=0
targets_dev_portnum["bitminer-ref"]=2
#------------------------------------

#------------------------------------
# kernel image define
declare -A kernel_image_name
kernel_image_name["s5p4418"]="zImage"
kernel_image_name["s5p6818"]="Image"

declare -A boot_partition_size
boot_partition_size["s5p4418"]=33554432 #32MB
boot_partition_size["s5p6818"]=67108864 #64MB
#------------------------------------

#------------------------------------
# RAM 1G USE
#mem_addrs=("${mem_1G_addrs[@]}")
# RAM 2G USE
#mem_addrs=("${mem_2G_addrs[@]}")
# RAM 512MB USE
#mem_addrs=("${mem_512MB_addrs[@]}")
# RAM 256MB USE
mem_addrs=("${mem_256MB_addrs[@]}")
#------------------------------------

# aes key
AES_KEY=
PRIVATE_KEY=${META_NEXELL_TOOLS_PATH}/secure_tools/private_key.pem
FIP_SEC_SIZE=
FIP_NONSEC_SIZE=

SECURE_BOOT="false"

MODULES_PATITION_SIZE=33554432

#------------------------------------
# BL2 -------------------------------
S5P4418_BL2_EMMC_LOAD_ADDR=0xb0fe0000
S5P4418_BL2_EMMC_JUMP_ADDR=0xb0fe0400

# COMMON ----------------------------
S5P4418_DISPATCHER_EMMC_LOAD_ADDR_COMMON=0xffff0200
S5P4418_DISPATCHER_EMMC_JUMP_ADDR_COMMON=0xffff0200

S5P4418_UBOOT_EMMC_LOAD_ADDR_COMMON=0x43c00000
S5P4418_UBOOT_EMMC_JUMP_ADDR_COMMON=0x43c00000

# SMARTVOICE ------------------------
S5P4418_UBOOT_EMMC_LOAD_ADDR_SMARTVOICE=0x74C00000
S5P4418_UBOOT_EMMC_JUMP_ADDR_SMARTVOICE=0x74C00000

# FFVOICE ------------------------
S5P4418_UBOOT_EMMC_LOAD_ADDR_FFVOICE=0xA4C00000
S5P4418_UBOOT_EMMC_JUMP_ADDR_FFVOICE=0xA4C00000

# DAUDIO ----------------------------
S5P4418_UBOOT_EMMC_LOAD_ADDR_DAUDIO_COVI=0x74C00000
S5P4418_UBOOT_EMMC_JUMP_ADDR_DAUDIO_COVI=0x74C00000
#------------------------------------

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
    echo "    ex) $0 s5p6818-avn-ref qt"
    echo "    ex) $0 s5p6818-avn-ref tiny"
    echo "    ex) $0 s5p4418-navi-ref qt"
    echo "    ex) $0 s5p4418-navi-ref tiny"
    echo "    ex) $0 s5p4418-smart-voice smartvoice"
}

function mem_addr_setup()
{
    MEM_LOAD_ADDR=${mem_addrs[0]}
    MEM_JUMP_ADDR=${mem_addrs[1]}
    MEM_SECURE_LOAD_ADDR=${mem_addrs[2]}
    MEM_SECURE_JUMP_ADDR=${mem_addrs[3]}
    MEM_NON_SECURE_LOAD_ADDR=${mem_addrs[4]}
    MEM_NON_SECURE_JUMP_ADDR=${mem_addrs[5]}
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
        #OPTEE_PLAT_DRAM_SIZE=2048
        mem_addrs=("${mem_256MB_addrs[@]}")
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

    cd $result_dir

    if [ ${MACHINE_NAME} == "s5p4418-ff-voice" -o ${IMAGE_TYPE} == "ubuntu" ]; then
        cp core-image-tiny-initramfs-${MACHINE_NAME}.cpio.gz initrd.gz
    else
        cp core-image-minimal-initramfs-${MACHINE_NAME}.cpio.gz initrd.gz
    fi

    ${META_NEXELL_CONVERT_TOOLS_PATH}/mkimage -A ${ARM_ARCH} -O linux -T ramdisk \
		             -C none -a 0 -e 0 -n uInitrd -d ${result_dir}/initrd.gz \
                             ${result_dir}/boot/uInitrd
    rm -f ${result_dir}/initrd.gz
    rm -rf ${ROOTDIR}
}

function mkparams()
{
    cd $result_dir
    ${META_NEXELL_CONVERT_TOOLS_PATH}/mkenvimage -s 16384 -o params.bin default_envs.txt
}

function mkbootimg()
{
    echo "================================================="
    echo "mkbootimg"
    echo "================================================="

    cd $result_dir
    cp -a ${kernel_image_name[${BOARD_SOCNAME}]} ./boot
    cp -a *.dtb ./boot
    cp ${META_NEXELL_TOOLS_PATH}/bootlogo/logo.bmp ./boot/

    #${META_NEXELL_CONVERT_TOOLS_PATH}/make_ext4fs -b 4096 -L boot -l ${boot_partition_size[${BOARD_SOCNAME}]} boot.img ./boot/
    ${META_NEXELL_CONVERT_TOOLS_PATH}/make_ext4fs -s -l ${boot_partition_size[${BOARD_SOCNAME}]} boot.img ./boot/
}

function make_2ndboot_for_emmc()
{
    echo "================================================="
    echo "make_2ndboot_for_emmc"
    echo "================================================="

    local bl1_source=
    local chip_name=${BOARD_SOCNAME}
    local gen_img=
    local aes_out_img=
    local aes_key=${1}

    if [ "${chip_name}" == "s5p6818" ]; then
	if [ ${SECURE_BOOT} == "true" ]; then
	    aes_key=${1}
	    echo -e "Yocto build not support aes encrypt! Something wrong!"
	    exit 1
        fi
	gen_img=bl1-emmcboot.bin
	aes_in_img=${gen_img}
	aes_out_img=bl1-emmcboot.img
    elif [ "${chip_name}" == "s5p4418" ]; then
	gen_img=bl1-emmcboot.bin
    else
	gen_img=bl1-emmcboot.img
    fi

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
    local soc_name=${1}
    local in=${2}
    local load_addr=${3}
    local jump_addr=${4}
    local out=${5}
    local extra_opts=${6}

    ${SECURE_TOOL} -c ${soc_name} -t 3rdboot -i ${in} -o ${out} -l ${load_addr} -e ${jump_addr} ${extra_opts}
}

function make_sparse_rootfs_img()
{
    echo "================================================="
    echo "make rootfs.img"
    echo "================================================="

    if [ "${IMAGE_TYPE}" == "ubuntu" ]; then
        ${META_NEXELL_CONVERT_TOOLS_PATH}/mkrootfs_image.sh \
            $result_dir \
            nexell-${IMAGE_TYPE}-${MACHINE_NAME}.ext4 \
            7288 \
            $result_dir/extra-rootfs-support
    fi

    #temporary, smartvoice can not use sparse image
    ${META_NEXELL_CONVERT_TOOLS_PATH}/ext2simg $result_dir/nexell-${IMAGE_TYPE}-${MACHINE_NAME}.ext4 $result_dir/rootfs.img
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

        make_2ndboot_for_emmc ${aes_key}

        gen_loader_emmc ${result_dir} fip-loader.bin ${PRIVATE_KEY} ${aes_key}
        gen_loader_sd ${result_dir} fip-loader.bin ${PRIVATE_KEY} ${aes_key}
        gen_secure ${result_dir} fip-secure.bin ${PRIVATE_KEY}
        gen_nonsecure ${result_dir} fip-nonsecure.bin ${PRIVATE_KEY}
	gen_loader_usb ${result_dir} fip-loader.bin ${PRIVATE_KEY} ${aes_key}
    else
	make_2ndboot_for_emmc
        # 1:${soc} |  2:${in} | 3:${load_addr} | 4:${jump_addr} | 5:${out} | 6:${extra_opts}
        local dev_portnum=${targets_dev_portnum[${BOARD_NAME}]}

        make_3rdboot_for_emmc ${BOARD_SOCNAME} \
                              ${result_dir}/pyrope-bl2.bin \
                              ${S5P4418_BL2_EMMC_LOAD_ADDR} \
                              ${S5P4418_BL2_EMMC_JUMP_ADDR} \
                              ${result_dir}/loader-emmc.img \
                              "-m 0x40200 -b 3 -p ${dev_portnum} -m 0x1E0200 -b 3 -p ${dev_portnum} -m 0x60200 -b 3 -p ${dev_portnum}"

        local dispatcher_emmc_load_addr=${S5P4418_DISPATCHER_EMMC_LOAD_ADDR_COMMON}
        local dispatcher_emmc_jump_addr=${S5P4418_DISPATCHER_EMMC_JUMP_ADDR_COMMON}
        local uboot_emmc_load_addr=${S5P4418_UBOOT_EMMC_LOAD_ADDR_COMMON}
        local uboot_emmc_jump_addr=${S5P4418_UBOOT_EMMC_JUMP_ADDR_COMMON}

        if [ ${BOARD_NAME} == "smart-voice" ];then
            uboot_emmc_load_addr=${S5P4418_UBOOT_EMMC_LOAD_ADDR_SMARTVOICE}
            uboot_emmc_jump_addr=${S5P4418_UBOOT_EMMC_JUMP_ADDR_SMARTVOICE}

        elif [ ${BOARD_NAME} == "ff-voice" ];then
            uboot_emmc_load_addr=${S5P4418_UBOOT_EMMC_LOAD_ADDR_FFVOICE}
            uboot_emmc_jump_addr=${S5P4418_UBOOT_EMMC_JUMP_ADDR_FFVOICE}
        else
            if [ ${BOARD_NAME} == "daudio-covi" ];then
                uboot_emmc_load_addr=${S5P4418_UBOOT_EMMC_LOAD_ADDR_DAUDIO_COVI}
                uboot_emmc_jump_addr=${S5P4418_UBOOT_EMMC_JUMP_ADDR_DAUDIO_COVI}
            fi
        fi

        make_3rdboot_for_emmc ${BOARD_SOCNAME} \
                          ${result_dir}/armv7_dispatcher.bin \
                          $dispatcher_emmc_load_addr \
                          $dispatcher_emmc_jump_addr \
                          ${result_dir}/bl_mon.img \
                          "-m 0x40200 -b 3 -p ${dev_portnum} -m 0x1E0200 -b 3 -p ${dev_portnum} -m 0x60200 -b 3 -p ${dev_portnum}"
        make_3rdboot_for_emmc ${BOARD_SOCNAME} \
                          ${result_dir}/u-boot.bin \
                          $uboot_emmc_load_addr \
                          $uboot_emmc_jump_addr \
                          ${result_dir}/bootloader.img

	if [ "${BOARD_SOCNAME}" == "s5p4418" ]; then
		local file_name=
		local pos=0
		local file_size=0

		file_name=${result_dir}/loader-emmc.img
		file_size=35840
		echo "fip-nonsecure-usb <-- loader-emmc.img size=${file_size} pos=${pos}"
		dd if=${result_dir}/loader-emmc.img of=${result_dir}/fip-nonsecure-usb.bin seek=0 bs=1
		let pos=pos+file_size

		file_name=${result_dir}/bl_mon.img
		file_size=28672
		echo "fip-nonsecure-usb <-- bl_mon.img size=${file_size} pos=${pos}"
		dd if=${result_dir}/bl_mon.img of=${result_dir}/fip-nonsecure-usb.bin seek=${pos} bs=1
		let pos=pos+file_size

		file_name=${result_dir}/bootloader.img
		echo -"fip-nonsecure-usb <-- bootloader.img size=${file_size} pos=${pos}"
		dd if=${result_dir}/bootloader.img of=${result_dir}/fip-nonsecure-usb.bin seek=${pos} bs=1
	fi

    fi

    echo -e "\n\033[0;34m ------------------------------------------------------------------------------------------ \033[0m\n"
    echo -e "\033[0;33m # FASTBOOT Download # \033[0m"
    echo -e "\033[0;33m    <FULL>                    \033[0m"
    echo -e "\033[0;36m    Run : \$ ${result_dir}/tools/standalone-fastboot-download.sh \033[0m"
    echo -e "\033[0;33m    <Kernel Only>             \033[0m"
    echo -e "\033[0;36m    Run : \$ ${result_dir}/tools/standalone-fastboot-download.sh -t kernel \033[0m"
    echo -e "\033[0;33m    <rootfs Only>             \033[0m"
    echo -e "\033[0;36m    Run : \$ ${result_dir}/tools/standalone-fastboot-download.sh -t rootfs \033[0m"
    echo -e "\033[0;33m    <More detail...>           \033[0m"
    echo -e "\033[0;36m    Run : \$ ${result_dir}/tools/standalone-fastboot-download.sh -h        \033[0m\n"

    echo -e "\033[0;33m # USB BOOT # \033[0m"
    echo -e "\033[0;36m    Run : \$ ${result_dir}/tools/standalone-uboot-by-usb-download.sh \033[0m\n"
    echo -e "\033[0;34m -------------------------------------------------------------------------------------------- \033[0m\n"
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

    local load_addr=$MEM_LOAD_ADDR
    local jump_addr=$MEM_JUMP_ADDR
    local bootdev=${4}
    local portnum=${5}

    local bl1_source=

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
	dev_offset_opts="-u -m $MEM_SECURE_LOAD_ADDR -z ${FIP_SEC_SIZE} \
		-m $MEM_NON_SECURE_LOAD_ADDR -z ${FIP_NONSEC_SIZE}"
    fi
    out_img="fip-loader-${devname}.img"
    aes_out_img="${out_img}"
    echo "[fip-loader] bootdev: ${bootdev}"
    echo "[fip-loader] out_img: ${out_img}"

    # SECURE_BINGEN
    ${SECURE_TOOL} -c ${chip_name} -t 3rdboot \
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

    local load_addr=$MEM_SECURE_LOAD_ADDR
    local jump_addr=$MEM_SECURE_JUMP_ADDR

    local bl1_source=

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

    # SECURE_BINGEN
    ${SECURE_TOOL} -c ${chip_name} -t 3rdboot \
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

    local load_addr=$MEM_NON_SECURE_LOAD_ADDR
    local jump_addr=$MEM_NON_SECURE_JUMP_ADDR

    local bl1_source=

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

    # SECURE_BINGEN
    ${SECURE_TOOL} -c ${chip_name} -t 3rdboot \
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
    ${META_NEXELL_CONVERT_TOOLS_PATH}/make_ext4fs -b 4096 -L modules -l ${MODULES_PATITION_SIZE} modules.img modules
}

function make_upgradefw() {
	if [ ! -e fw ]; then
		mkdir -p fw
	else
		rm -rf fw/*
	fi

	BUILD_SERVER_IP=`hostname -I | awk '{print $1}'`
	FWUP_FILENAME=bitminer_FW_$(date +"%Y%m%d_%H%M%S")_"${BUILD_SERVER_IP}"_$(whoami)
	echo "FWUP_FILENAME=${FWUP_FILENAME}" >> fw/fwup_cmd.sh
	echo "FWUP_HISTORY_DIR=/fwup_history/\$(date +\"%Y%m%d_%H%M\").${FWUP_FILENAME}" >> fw/fwup_cmd.sh
	echo "mkdir -p /config/\${FWUP_HISTORY_DIR}" >> fw/fwup_cmd.sh
	echo "fw_setenv rw_bl1 \"ext4write mmc 0:5 0x40000000 \${FWUP_HISTORY_DIR}/bl1 1 0; mmc write 40000000    1   80\"" >> fw/fwup_cmd.sh
	echo "fw_setenv rw_ldr \"ext4write mmc 0:5 0x40000000 \${FWUP_HISTORY_DIR}/ldr 1 0; mmc write 40000000   81  280\"" >> fw/fwup_cmd.sh
	echo "fw_setenv rw_sec \"ext4write mmc 0:5 0x40000000 \${FWUP_HISTORY_DIR}/sec 1 0; mmc write 40000000  301  c00\"" >> fw/fwup_cmd.sh
	echo "fw_setenv rw_nsc \"ext4write mmc 0:5 0x40000000 \${FWUP_HISTORY_DIR}/nsc 1 0; mmc write 40000000  f01  800\"" >> fw/fwup_cmd.sh
	echo "fw_setenv rw_prm \"ext4write mmc 0:5 0x40000000 \${FWUP_HISTORY_DIR}/prm 1 0; mmc write 40000000 1701   20\"" >> fw/fwup_cmd.sh
	echo "fw_setenv rw_bt  \"ext4write mmc 0:5 0x40000000 \${FWUP_HISTORY_DIR}/bt  1 0;\" ' ext4_img_write 0 0000000040000000 1 \$filesize'" >> fw/fwup_cmd.sh
	echo "fw_setenv rw_fs  \"ext4write mmc 0:5 0x40000000 \${FWUP_HISTORY_DIR}/fs  1 0;\" ' ext4_img_write 0 0000000040000000 3 \$filesize'" >> fw/fwup_cmd.sh

	echo "fw_setenv up_bl1 'if ext4load mmc 0:5 0x40000000 /fw/bl1-emmcboot.img;    then run rw_bl1; fi;'" >> fw/fwup_cmd.sh
	echo "fw_setenv up_ldr 'if ext4load mmc 0:5 0x40000000 /fw/fip-loader-emmc.img; then run rw_ldr; fi;'" >> fw/fwup_cmd.sh
	echo "fw_setenv up_sec 'if ext4load mmc 0:5 0x40000000 /fw/fip-secure.img;      then run rw_sec; fi;'" >> fw/fwup_cmd.sh
	echo "fw_setenv up_nsc 'if ext4load mmc 0:5 0x40000000 /fw/fip-nonsecure.img;   then run rw_nsc; fi;'" >> fw/fwup_cmd.sh
	echo "fw_setenv up_prm 'if ext4load mmc 0:5 0x40000000 /fw/params.bin;          then run rw_prm; fi;'" >> fw/fwup_cmd.sh
	echo "fw_setenv up_bt  'if ext4load mmc 0:5 0x40000000 /fw/boot.img;            then run rw_bt ; fi;'" >> fw/fwup_cmd.sh
	echo "fw_setenv up_fs  'if ext4load mmc 0:5 0x40000000 /fw/rootfs.img;          then run rw_fs ; fi;'" >> fw/fwup_cmd.sh

	echo "fw_setenv rm_bl1 'setenv up_bl1; setenv rw_bl1; setenv rm_bl1'" >> fw/fwup_cmd.sh
	echo "fw_setenv rm_ldr 'setenv up_ldr; setenv rw_ldr; setenv rm_ldr'" >> fw/fwup_cmd.sh
	echo "fw_setenv rm_sec 'setenv up_sec; setenv rw_sec; setenv rm_sec'" >> fw/fwup_cmd.sh
	echo "fw_setenv rm_nsc 'setenv up_nsc; setenv rw_nsc; setenv rm_nsc'" >> fw/fwup_cmd.sh
	echo "fw_setenv rm_prm 'setenv up_prm; setenv rw_prm; setenv rm_prm'" >> fw/fwup_cmd.sh
	echo "fw_setenv rm_bt  'setenv up_bt ; setenv rw_bt ; setenv rm_bt '" >> fw/fwup_cmd.sh
	echo "fw_setenv rm_fs  'setenv up_fs ; setenv rw_fs ; setenv rm_fs '" >> fw/fwup_cmd.sh
	echo "fw_setenv rm_fw  'setenv rm_fw ; setenv clr_fwupenv ; setenv do_fw '" >> fw/fwup_cmd.sh
	echo "fw_setenv rm_restore 'setenv rm_restore; setenv restore_bootcmd '" >> fw/fwup_cmd.sh

	echo "fw_setenv clr_fwupenv 'run rm_bl1 rm_ldr rm_sec rm_nsc rm_prm rm_bt rm_fs rm_restore rm_fw'" >> fw/fwup_cmd.sh

	echo "fw_setenv restore_bootcmd 'setenv bootcmd \"run mmcboot\"'" >> fw/fwup_cmd.sh

	echo "fw_setenv do_fw 'run up_bl1 up_ldr up_sec up_nsc up_prm up_bt up_fs restore_bootcmd clr_fwupenv; saveenv'" >> fw/fwup_cmd.sh

	echo "fw_setenv bootcmd 'run do_fw; reset'" >> fw/fwup_cmd.sh

	cp -ua bl1-emmcboot.img		fw/
	cp -ua fip-loader-emmc.img	fw/
	cp -ua fip-secure.img		fw/
	cp -ua fip-nonsecure.img		fw/
#	cp -ua params.bin			fw/
	cp -ua boot.img				fw/
#	cp -ua modules.img			fw/
	cp -ua rootfs.img			fw/
	tar czf ${FWUP_FILENAME}.tar.gz fw
}

function make_bootsd() {
	if [ -e make_bootsd.sh ]; then
		rm -rf make_bootsd.sh
	fi
	echo "#!/bin/sh" >> make_bootsd.sh
	echo "sudo dd if=bl1-sdboot.bin of=/dev/sdd seek=512 bs=1" >> make_bootsd.sh;
	echo "sudo dd if=fip-loader-sd.img of=/dev/sdd seek=66048 bs=1" >> make_bootsd.sh
	echo "sudo dd if=fip-secure.img of=/dev/sdd seek=393728 bs=1" >> make_bootsd.sh
	echo "sudo dd if=fip-nonsecure.img of=/dev/sdd seek=1966592 bs=1" >> make_bootsd.sh
	echo "sudo dd if=params.bin of=/dev/sdd seek=3015168 bs=1" >> make_bootsd.sh
	echo "sync" >> make_bootsd.sh
	chmod +x make_bootsd.sh
}

echo -e "\n\033[0;34m ------------------------------------------------------------------ \033[0m"
echo -e "\033[0;36m                      Convert images Running                        \033[0m"
echo -e "\033[0;34m ------------------------------------------------------------------ \033[0m"

check_usage
convert_env_setup
make_dirs
mkramdisk
mkparams
mkbootimg
mem_addr_setup
make_sparse_rootfs_img
post_process
#make_modules
make_upgradefw
make_bootsd
