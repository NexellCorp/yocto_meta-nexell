require linux-nexell.inc
inherit linux-nexell-base

DESCRIPTION = "Linux Kernel for nexell DAUDIO"

COMPATIBLE_MACHINE = "s5p4418-daudio-ref"

DEPENDS = "s5p4418-daudio-ref-uboot s5p4418-daudio-ref-bl1 s5p4418-daudio-ref-bl2 s5p4418-daudio-ref-dispatcher"

KERNEL_DEFCONFIG_s5p4418-daudio-ref ?= "s5p4418_navi_ref_defconfig"
KERNEL_COMMON_FLAGS = "ARCH=arm CROSS_COMPILE=${TARGET_PREFIX}"

ARM_ARCH = "arm"
CHIP_NAME = "s5p4418"
BOARD_PREFIX = "navi_ref"

do_kernel_configme_prepend() {
    config=${S}/arch/${ARM_ARCH}/configs/${KERNEL_DEFCONFIG}

    echo "CONFIG_FRAMEBUFFER_CONSOLE=y" >> $config
    echo "CONFIG_PINCTRL_NEXELL=y" >> $config
    echo "CONFIG_FONTS=y" >> $config
    echo "CONFIG_HID_GENERIC=y" >> $config
    echo "CONFIG_HIDRAW=y" >> $config

    install -m 0644 ${S}/arch/${ARM_ARCH}/configs/${KERNEL_DEFCONFIG} ${WORKDIR}/defconfig || die "No default configuration for ${MACHINE} / ${KERNEL_DEFCONFIG} available."
}

do_compile() {
#    oe_runmake ARCH=${ARM_ARCH} distclean
    local dts_file=${S}/arch/${ARM_ARCH}/boot/dts/${CHIP_NAME}-${BOARD_PREFIX}-rev00.dts

    oe_runmake ARCH=${ARM_ARCH} ${KERNEL_DEFCONFIG}

    oe_runmake ${KERNEL_COMMON_FLAGS} zImage
    oe_runmake ${KERNEL_COMMON_FLAGS} dtbs
    oe_runmake ${KERNEL_COMMON_FLAGS} modules
    
    oe_runmake ${KERNEL_COMMON_FLAGS} modules_install INSTALL_MOD_PATH=${D} INSTALL_MOD_STRIP=1
}

do_install_append() {
    echo "${KBUILD_OUTPUT}" > ${BASE_WORKDIR}/source_kernel_dir_path.txt
}