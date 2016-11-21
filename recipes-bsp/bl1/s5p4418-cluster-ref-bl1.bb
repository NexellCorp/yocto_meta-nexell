require nexell-bl1-s5p4418-common.inc

COMPATIBLE_MACHINE = "s5p4418-cluster-ref"

TOOLCHAIN_ARCH32_EABI = "${BASE_WORKDIR}/arm-eabi-4.8/bin/"

EXTRA_OEMAKE = "\
    'VPATH=${WORKDIR}/git' \
"

do_compile () {
    oe_runmake CROSS_TOOL_TOP=${TOOLCHAIN_ARCH32_EABI} BOARD="NAVI" CHIPNAME="NXP4330" -j 1
}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
	mv ${S}/out/bl1-navi.bin ${S}/out/bl1-cluster.bin
    install -m 0644 ${S}/out/bl1-cluster.bin ${DEPLOY_DIR_IMAGE}
}

addtask deploy after do_install
