require linux-nexell.inc
inherit linux-nexell-base
inherit nexell-optee-preprocess

DESCRIPTION = "Linux Kernel for nexell artik710-raptor"
SECTION = "kernel"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

SRC_URI += " \
        file://defconfig \
        "

COMPATIBLE_MACHINE = "s5p6818-artik710-raptor"

DEPENDS = "optee-build"

PE = "1"
PV = "4.1.15"

PROVIDES += "virtual/kernel kernel kernel-base kernel-image kernel-dev kernel-vmlinux kernel-misc kernel-modules"

KERNEL_DEFCONFIG_s5p6818-artik710-raptor ?= "artik710_raptor_defconfig"

KERNEL_COMMON_FLAGS = "ARCH=arm64 CROSS_COMPILE=${TARGET_PREFIX}"

PATH_KERNEL_SRC = "${@env_setup(d,"kernel-source")}"
PATH_KERNEL_BUILD = "${@env_setup(d,"kernel-build-artifacts")}"

ARM_ARCH = "arm64"
CHIP_NAME = "s5p6818"
BOARD_PREFIX = "artik710"

do_kernel_configme_prepend() {
    install -m 0644 ${S}/arch/${ARM_ARCH}/configs/${KERNEL_DEFCONFIG} ${WORKDIR}/defconfig || die "No default configuration for ${MACHINE} / ${KERNEL_DEFCONFIG} available."
}

do_compile() {
    oe_runmake ARCH=arm64 distclean
    local dts_file=${S}/arch/${ARM_ARCH}/boot/dts/nexell/${CHIP_NAME}-${BOARD_PREFIX}.dtsi

    oe_runmake ARCH=${ARM_ARCH} ${KERNEL_DEFCONFIG}

    export LDFLAGS="-O1 --hash-style=gnu --as-needed"

    local memreserve_size=0x7ed00000
    local reg_size=0x3ee00000

    sed -i -e 's/\(\/memreserve\/[[:blank:]]\+\)\([[:alnum:]]\+[[:blank:]]\+\)\([[:alnum:]]\+\)/\1'"${memreserve_size}"' \3/' ${dts_file} 
    sed -i -e 's/\(^[[:blank:]]\+reg = <0x40000000 \)[[:alnum:]]\+>/\1'"${reg_size}"'>/' ${dts_file}   

    oe_runmake ${KERNEL_COMMON_FLAGS} Image -j8
    oe_runmake ${KERNEL_COMMON_FLAGS} dtbs
    oe_runmake ${KERNEL_COMMON_FLAGS} modules -j8
    
    oe_runmake ${KERNEL_COMMON_FLAGS} modules_install INSTALL_MOD_PATH=${D} INSTALL_MOD_STRIP=1
}

#For optee-linuxdriver
do_install_append() {
    echo "${KBUILD_OUTPUT}" > ${BASE_WORKDIR}/source_kernel_dir_path.txt

    cp -a ${PATH_KERNEL_BUILD}/.config ${PATH_KERNEL_SRC}
    cp -a ${PATH_KERNEL_BUILD}/* ${PATH_KERNEL_SRC}

    cp -a ${PATH_KERNEL_SRC}/Makefile ${KBUILD_OUTPUT}
    cp -a ${PATH_KERNEL_SRC}/scripts ${KBUILD_OUTPUT}
    cp -a ${PATH_KERNEL_SRC}/arch/arm ${KBUILD_OUTPUT}/arch
    cp -a ${PATH_KERNEL_SRC}/arch/arm64 ${KBUILD_OUTPUT}/arch
    cp -a ${PATH_KERNEL_SRC}/include ${KBUILD_OUTPUT}
}