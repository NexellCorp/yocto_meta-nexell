require bl1-secure.inc

LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://README;md5=754608f69d5791d96a0a96281ae48814"

SRCREV = "fb9ddd1beb794a5cf2bc250205e392bf145d6eb7"
SRC_URI = "git://git.nexell.co.kr/nexell/bl1/bl1-s5p6818;protocol=git;branch=nexell"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = "arm-eabi-4.8"
TOOLCHAIN_ARCH32_EABI = "${RECIPE_SYSROOT}${datadir}/arm-eabi-4.8-toolchain/bin/"

EXTRA_OEMAKE = "\
    'VPATH=${WORKDIR}/git' \
"

do_compile () {
    oe_runmake CROSS_COMPILE_TOP=${TOOLCHAIN_ARCH32_EABI} ${BL1_BUILD_CONFIG} ${SECURE-BL1} -j 1
}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/out/${BL1_BIN} ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/out/${BL1_EMMCBOOT} ${DEPLOY_DIR_IMAGE}
}

addtask deploy after do_install
