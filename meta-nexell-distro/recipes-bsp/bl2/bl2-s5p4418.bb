LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://pyrope_bl2.lds;md5=c81ae199c8ac9bc25a4389a9c6b387b6"
SRCREV = "12675947b00e416ea4775cafa29748cea04f4f24"
SRC_URI = "git://git.nexell.co.kr/nexell/bl2/bl2-s5p4418;protocol=git;branch=nexell"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

EXTRA_OEMAKE = "\
    'VPATH=${WORKDIR}/git' \
"

do_compile () {
    cd ${S}
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} QUICKBOOT=${QUICKBOOT_ENABLE} -j 1
}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/out/pyrope-bl2.bin ${DEPLOY_DIR_IMAGE}
}

addtask deploy after do_install
