require nexell-bl1-s5p4418-common.inc

COMPATIBLE_MACHINE = "s5p4418-avn-ref"

EXTRA_OEMAKE = "\
    'VPATH=${WORKDIR}/git' \
"

do_compile () {
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CHIPNAME="s5p4418" BOARD="avn" KERNEL_VER="4" ARM_SECURE="n" DEVICE_PORT="2" SYSLOG="n" -j 1
}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/out/bl1-avn.bin ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/out/bl1-emmcboot.bin ${DEPLOY_DIR_IMAGE}
}

addtask deploy after do_install
