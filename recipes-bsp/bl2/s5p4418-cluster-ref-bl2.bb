require nexell-bl2-s5p4418-common.inc

COMPATIBLE_MACHINE = "s5p4418-cluster-ref"

EXTRA_OEMAKE = "\
    'VPATH=${WORKDIR}/git' \
"

do_compile () {
    cd ${S}
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} QUICKBOOT=1 -j 1
}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/out/pyrope-bl2.bin ${DEPLOY_DIR_IMAGE}
}

addtask deploy after do_install
