require linux-nexell.inc
inherit linux-nexell-base

DESCRIPTION = "Linux Kernel for nexell artik530-raptor"
SECTION = "kernel"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

SRC_URI += " \
        file://defconfig \
        "

COMPATIBLE_MACHINE = "artik530-raptor"

DEPENDS = "bl1-artik530 u-boot-artik530"

PE = "1"
PV = "4.1.15"

KERNEL_DEFCONFIG_artik530-raptor ?= "artik530_raptor_defconfig"

#UDEV_GE_141 ?= "1"

KERNEL_COMMON_FLAGS = "ARCH=arm CROSS_COMPILE=${TARGET_PREFIX}"

ARM_ARCH = "arm"
CHIP_NAME = "s5p4418"
BOARD_PREFIX = "artik530"

do_kernel_configme_prepend() {
    config=${S}/arch/${ARM_ARCH}/configs/${KERNEL_DEFCONFIG}

    install -m 0644 ${S}/arch/arm/configs/${KERNEL_DEFCONFIG} ${WORKDIR}/defconfig || die "No default configuration for ${MACHINE} / ${KERNEL_DEFCONFIG} available."

}

do_compile() {
    echo -e "\033[44;33m << ----debug suker---- do_compile start >>\033[0m"
    oe_runmake ARCH=arm distclean
    local dts_file=${S}/arch/${ARM_ARCH}/boot/dts/${CHIP_NAME}-${BOARD_PREFIX}.dtsi

    echo -e "\033[44;33m << ----debug suker---- KERNEL_DEFCONFIG ==> ${KERNEL_DEFCONFIG} >>\033[0m"
    oe_runmake ARCH=${ARM_ARCH} ${KERNEL_DEFCONFIG}

    export LDFLAGS="-O1 --hash-style=gnu --as-needed"

    local memreserve_size=0x7ed00000
    local reg_size=0x3ee00000

    echo "memreserve_size --> ${memreserve_size}"
    echo "reg_size --> ${reg_size}"

    sed -i -e 's/\(\/memreserve\/[[:blank:]]\+\)\([[:alnum:]]\+[[:blank:]]\+\)\([[:alnum:]]\+\)/\1'"${memreserve_size}"' \3/' ${dts_file} 
    sed -i -e 's/\(^[[:blank:]]\+reg = <0x40000000 \)[[:alnum:]]\+>/\1'"${reg_size}"'>/' ${dts_file}   

    oe_runmake ${KERNEL_COMMON_FLAGS} Image -j8
    oe_runmake ${KERNEL_COMMON_FLAGS} dtbs
    oe_runmake ${KERNEL_COMMON_FLAGS} modules -j8
    
    oe_runmake ${KERNEL_COMMON_FLAGS} modules_install INSTALL_MOD_PATH=${D}/lib/modules INSTALL_MOD_STRIP=1
}