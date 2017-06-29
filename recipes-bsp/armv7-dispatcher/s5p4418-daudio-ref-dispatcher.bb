require nexell-dispatcher-s5p4418-common.inc

COMPATIBLE_MACHINE = "s5p4418-daudio-ref"

EXTRA_OEMAKE = "\
    'VPATH=${WORKDIR}/git' \
"

do_compile () {
    cd ${S}
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} BOARD="NAVI" -j 1
}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/out/armv7_dispatcher-navi.bin ${DEPLOY_DIR_IMAGE}/armv7_dispatcher-daudio.bin
}

addtask deploy after do_install
