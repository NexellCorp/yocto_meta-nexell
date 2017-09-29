require linux-nexell.inc
inherit linux-nexell-base
inherit nexell-source-path-extension

KCONFIG_MODE="--alldefconfig"

#PATH_KERNEL_SRC = "${@env_setup(d,"kernel-source")}"
#PATH_KERNEL_BUILD = "${@env_setup(d,"kernel-build-artifacts")}"

DEPENDS += "optee-build"

#For optee-linuxdriver
do_kernelpostprocess() {
    echo "${KBUILD_OUTPUT}" > ${BASE_WORKDIR}/LINUX_STANDARD_BUILD_PATH.txt
    echo "${KBUILD_DEFCONFIG}" > ${BASE_WORKDIR}/KBUILD_DEFCONFIG.txt
    echo "${KERNEL_DEVICETREE}" > ${BASE_WORKDIR}/KBUILD_DEVICETREE.txt

    rm -rf ${BASE_WORKDIR}/temp_kernel_out
    mkdir -p ${BASE_WORKDIR}/temp_kernel_out

    cp -an ${KBUILD_OUTPUT}/* ${BASE_WORKDIR}/temp_kernel_out
    cp -a ${STAGING_KERNEL_BUILDDIR}/.config ${BASE_WORKDIR}/temp_kernel_out
    cp -a ${STAGING_KERNEL_BUILDDIR}/* ${BASE_WORKDIR}/temp_kernel_out
    cp -a ${STAGING_KERNEL_DIR}/Makefile ${BASE_WORKDIR}/temp_kernel_out
    cp -a ${STAGING_KERNEL_DIR}/scripts ${BASE_WORKDIR}/temp_kernel_out
    cp -a ${STAGING_KERNEL_DIR}/arch/arm ${BASE_WORKDIR}/temp_kernel_out/arch
    cp -a ${STAGING_KERNEL_DIR}/arch/arm64 ${BASE_WORKDIR}/temp_kernel_out/arch
    cp -a ${STAGING_KERNEL_DIR}/include ${BASE_WORKDIR}/temp_kernel_out
}

addtask kernelpostprocess before do_deploy after do_install
