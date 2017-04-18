require linux-nexell.inc
inherit linux-nexell-base
inherit nexell-optee-preprocess

DESCRIPTION = "Linux Kernel for nexell kick-st"

COMPATIBLE_MACHINE = "s5p6818-kick-st"

DEPENDS = "optee-build"

PROVIDES += "virtual/kernel kernel kernel-base kernel-image kernel-dev kernel-vmlinux kernel-misc kernel-modules"

KERNEL_DEFCONFIG_s5p6818-kick-st ?= "s5p6818_kick_st_defconfig"

KERNEL_COMMON_FLAGS = "ARCH=arm64 CROSS_COMPILE=${TARGET_PREFIX}"

PATH_KERNEL_SRC = "${@env_setup(d,"kernel-source")}"
PATH_KERNEL_BUILD = "${@env_setup(d,"kernel-build-artifacts")}"

ARM_ARCH = "arm64"
CHIP_NAME = "s5p6818"
BOARD_PREFIX = "kick-st"

do_kernel_configme_prepend() {
    config=${S}/arch/arm64/configs/${KERNEL_DEFCONFIG}

    install -m 0644 ${S}/arch/${ARM_ARCH}/configs/${KERNEL_DEFCONFIG} ${WORKDIR}/defconfig || die "No default configuration for ${MACHINE} / ${KERNEL_DEFCONFIG} available."
}

do_compile() {
#    oe_runmake ARCH=arm64 distclean
    local dts_file=${S}/arch/${ARM_ARCH}/boot/dts/nexell/${CHIP_NAME}-${BOARD_PREFIX}-common.dtsi

    oe_runmake ARCH=${ARM_ARCH} ${KERNEL_DEFCONFIG}

    export LDFLAGS="-O1 --hash-style=gnu --as-needed"

    oe_runmake ${KERNEL_COMMON_FLAGS} Image -j8
    oe_runmake ${KERNEL_COMMON_FLAGS} dtbs
    oe_runmake ${KERNEL_COMMON_FLAGS} modules -j8

    oe_runmake ${KERNEL_COMMON_FLAGS} modules_install INSTALL_MOD_PATH=${D} INSTALL_MOD_STRIP=1
}

#For optee-linuxdriver
do_install_append() {
    echo "${KBUILD_OUTPUT}" > ${BASE_WORKDIR}/source_kernel_dir_path.txt

    rm -rf ${BASE_WORKDIR}/temp_kernel_out
    mkdir -p ${BASE_WORKDIR}/temp_kernel_out

    cp -an ${KBUILD_OUTPUT}/* ${BASE_WORKDIR}/temp_kernel_out

    cp -a ${PATH_KERNEL_BUILD}/.config ${BASE_WORKDIR}/temp_kernel_out
    cp -a ${PATH_KERNEL_BUILD}/* ${BASE_WORKDIR}/temp_kernel_out
    cp -a ${PATH_KERNEL_SRC}/Makefile ${BASE_WORKDIR}/temp_kernel_out
    cp -a ${PATH_KERNEL_SRC}/scripts ${BASE_WORKDIR}/temp_kernel_out
    cp -a ${PATH_KERNEL_SRC}/arch/arm ${BASE_WORKDIR}/temp_kernel_out/arch
    cp -a ${PATH_KERNEL_SRC}/arch/arm64 ${BASE_WORKDIR}/temp_kernel_out/arch
    cp -a ${PATH_KERNEL_SRC}/include ${BASE_WORKDIR}/temp_kernel_out
}
