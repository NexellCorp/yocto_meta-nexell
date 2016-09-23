require linux-nexell.inc
inherit linux-nexell-base

DESCRIPTION = "Linux Kernel for nexell NAVI"
SECTION = "kernel"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

SRC_URI += " \
        file://defconfig \
        "

COMPATIBLE_MACHINE = "s5p4418-cluster-ref"

DEPENDS = "s5p4418-cluster-ref-uboot s5p4418-cluster-ref-bl1"

PE = "1"
PV = "4.1.15"

KERNEL_DEFCONFIG_s5p4418-cluster-ref ?= "s5p4418_cluster_ref_defconfig"

#UDEV_GE_141 ?= "1"
KERNEL_MODULE_AUTOLOAD += "${@bb.utils.contains("MACHINE_FEATURES", "pitft28r", "stmpe-ts", "", d)}"

KERNEL_COMMON_FLAGS = "ARCH=arm CROSS_COMPILE=${TARGET_PREFIX}"

ARM_ARCH = "arm"
CHIP_NAME = "s5p4418"
BOARD_PREFIX = "cluster_ref"

do_kernel_configme_prepend() {
    config=${S}/arch/arm/configs/${KERNEL_DEFCONFIG}

    sed -i "/CONFIG_MEDIA_SUBDRV_AUTOSELECT[ =]/d" $config
    echo "CONFIG_MEDIA_SUBDRV_AUTOSELECT=n" >> $config

    sed -i "/CONFIG_CMA_SIZE_MBYTES[ =]/d" $config
    echo "CONFIG_CMA_SIZE_MBYTES=256" >> $config

	echo "CONFIG_MALI400=m" >> $config

    install -m 0644 ${S}/arch/arm/configs/${KERNEL_DEFCONFIG} ${WORKDIR}/defconfig || die "No default configuration for ${MACHINE} / ${KERNEL_DEFCONFIG} available."
}

do_compile() {
    oe_runmake ARCH=arm distclean
    local dts_file=${S}/arch/${ARM_ARCH}/boot/dts/${CHIP_NAME}-${BOARD_PREFIX}-rev00.dts

    oe_runmake ARCH=${ARM_ARCH} ${KERNEL_DEFCONFIG}

    export LDFLAGS="-O1 --hash-style=gnu --as-needed"

    local memreserve_size=0x7ed00000
    local reg_size=0x3ee00000

    sed -i -e 's/\(\/memreserve\/[[:blank:]]\+\)\([[:alnum:]]\+[[:blank:]]\+\)\([[:alnum:]]\+\)/\1'"${memreserve_size}"' \3/' ${dts_file} 
    sed -i -e 's/\(^[[:blank:]]\+reg = <0x40000000 \)[[:alnum:]]\+>/\1'"${reg_size}"'>/' ${dts_file}   

    oe_runmake ${KERNEL_COMMON_FLAGS} zImage -j8
    oe_runmake ${KERNEL_COMMON_FLAGS} dtbs
    oe_runmake ${KERNEL_COMMON_FLAGS} modules -j8
    
    oe_runmake ${KERNEL_COMMON_FLAGS} modules_install INSTALL_MOD_PATH=${D} INSTALL_MOD_STRIP=1
}
