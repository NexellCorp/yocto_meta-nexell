require linux-nexell.inc
inherit linux-nexell-base

KCONFIG_MODE="--alldefconfig"
ARM_ARCH = "arm"
DEPENDS += "u-boot-nexell bl1-s5p4418 bl2-s5p4418 dispatcher-s5p4418"

do_kernelpostprocess() {
    echo "${KBUILD_OUTPUT}" > ${BASE_WORKDIR}/KBUILD_OUTPUT_PATH.txt
    echo "${KBUILD_DEFCONFIG}" > ${BASE_WORKDIR}/KBUILD_DEFCONFIG.txt
    echo "${KERNEL_DEVICETREE}" > ${BASE_WORKDIR}/KBUILD_DEVICETREE.txt
}

addtask kernelpostprocess before do_deploy after do_install