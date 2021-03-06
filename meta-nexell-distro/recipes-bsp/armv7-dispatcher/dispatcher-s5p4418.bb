require dispatcher-s5p4418.inc

EXTRA_OEMAKE = "\
    'VPATH=${WORKDIR}/git' \
"

do_compile () {
    cd ${S}
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} ${DISPATCHER_BUILD_CONFIG} -j 1
}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/out/${DISPATCHER_BIN} ${DEPLOY_DIR_IMAGE}
}

addtask deploy after do_install
