include bl1-secure.inc
require nexell-bl1-s5p6818-common.inc

COMPATIBLE_MACHINE = "s5p6818-artik710-raptor"

TOOLCHAIN_ARCH32_EABI = "${BASE_WORKDIR}/arm-eabi-4.8/bin/"

EXTRA_OEMAKE = "\
    'VPATH=${WORKDIR}/git' \
"

do_compile () {
    oe_runmake CROSS_TOOL_TOP=${TOOLCHAIN_ARCH32_EABI} BOARD="raptor" KERNEL_VER="4" DEVICE_PORT="2" SYSLOG="n" ${SECURE-BL1} -j 1
}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/out/bl1-raptor.bin ${DEPLOY_DIR_IMAGE}
}

addtask deploy after do_install
