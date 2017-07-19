LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://README;md5=c48cff868868eb40f73e510f7bae7710"
SRCREV = "ce9b7dff8a8515d2e5ae6a9363fb7e21c6841960"
SRC_URI = "git://git.nexell.co.kr/nexell/bl1/bl1-s5p4418;protocol=git;branch=nexell"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

EXTRA_OEMAKE = "\
    'VPATH=${WORKDIR}/git' \
"

do_compile () {
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} ${BL1_BUILD_CONFIG} -j 1
}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/out/${BL1_BIN} ${DEPLOY_DIR_IMAGE}/${BL1_FINAL_BIN}
    install -m 0644 ${S}/out/${BL1_EMMCBOOT} ${DEPLOY_DIR_IMAGE}
}

addtask deploy after do_install
