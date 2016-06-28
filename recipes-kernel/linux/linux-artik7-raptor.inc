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

COMPATIBLE_MACHINE = "artik710-raptor"

DEPENDS = "optee-build"

PE = "1"
PV = "4.1.15"

KERNEL_DEFCONFIG_artik710-raptor ?= "artik710_raptor_defconfig"

UDEV_GE_141 ?= "1"
KERNEL_MODULE_AUTOLOAD += "${@bb.utils.contains("MACHINE_FEATURES", "pitft28r", "stmpe-ts", "", d)}"

KERNEL_COMMON_FLAGS = "ARCH=arm64 CROSS_COMPILE=${TARGET_PREFIX}"

PATH_KERNEL_SRC = "${@env_setup(d,"kernel-source")}"
PATH_KERNEL_BUILD = "${@env_setup(d,"kernel-build-artifacts")}"

ARM_ARCH = "arm64"
CHIP_NAME = "s5p6818"
BOARD_PREFIX = "artik710"

# python __anonymous () {
#     kerneltype = d.getVar('KERNEL_IMAGETYPE', True)
#     kerneldt = get_dts(d, d.getVar('LINUX_VERSION', True))
#     d.setVar("KERNEL_DEVICETREE", kerneldt)
# }

do_kernel_configme_prepend() {
    install -m 0644 ${S}/arch/${ARM_ARCH}/configs/${KERNEL_DEFCONFIG} ${WORKDIR}/defconfig || die "No default configuration for ${MACHINE} / ${KERNEL_DEFCONFIG} available."
}

do_mypatch() {
    git fetch ssh://suker@59.13.55.140:29418/linux-artik7 refs/changes/32/2132/2 && git cherry-pick FETCH_HEAD;
    git fetch ssh://suker@59.13.55.140:29418/linux-artik7 refs/changes/17/2217/2 && git cherry-pick FETCH_HEAD
    echo "nothing"
}

do_compile() {
    oe_runmake ARCH=arm64 distclean
    local dts_file=${S}/arch/${ARM_ARCH}/boot/dts/nexell/${CHIP_NAME}-${BOARD_PREFIX}.dtsi

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

#For optee-linuxdriver
do_install_append() {
    cp -a ${PATH_KERNEL_BUILD}/.config ${PATH_KERNEL_SRC}
    cp -a ${PATH_KERNEL_BUILD}/* ${PATH_KERNEL_SRC}

    cp -a ${PATH_KERNEL_SRC}/Makefile ${KBUILD_OUTPUT}
    cp -a ${PATH_KERNEL_SRC}/scripts ${KBUILD_OUTPUT}
    cp -a ${PATH_KERNEL_SRC}/arch/arm ${KBUILD_OUTPUT}/arch
    cp -a ${PATH_KERNEL_SRC}/arch/arm64 ${KBUILD_OUTPUT}/arch
    cp -a ${PATH_KERNEL_SRC}/include ${KBUILD_OUTPUT}
}

addtask mypatch after do_unpack