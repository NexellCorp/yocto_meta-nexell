LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://pyrope_bl2.lds;md5=c81ae199c8ac9bc25a4389a9c6b387b6"
PV ?= "1.0+git${SRCPV}"
SRCREV = "${AUTOREV}"

SRC_URI = "git://review.gerrithub.io/NexellCorp/bl2_s5p4418;protocol=https;branch=nexell \
           file://0001-bl2-link-error-__aeabi_idiv.patch"

S = "${WORKDIR}/git"

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
