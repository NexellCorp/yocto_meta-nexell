#SECTION = "bootloader"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

#SRCREV = "4ff225bf6fd1c37a2a61aca157c0e497ab6e6d42"
#SRC_URI = "git://git.nexell.co.kr/nexell/bl1/bl1-s5p6818;protocol=git;branch=artik \
#"

SRC_URI[md5sum] = "d7572d651449671c8ceba7d634c0fd42"
SRC_URI[sha256sum] = "7a50508e0a3aa9ea47783dea1f40db1cd7156a210e6c87e4e6363ce776b0dc5b"
SRCREV = "6b8b1b4ad3b0607e4054af42b7a37e722ed06e89"
SRC_URI = "git://github.com/kchhero/bl1_test.git"

# Modify these as desired
#PV = "1.0+git${SRCPV}"

S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"
COMPATIBLE_MACHINE = "artik710-raptor"

EXTRA_OEMAKE = "\
    'VPATH=${WORKDIR}/git' \
"

do_compile () {
    echo -e "\033[44;33m WORKDIR ==> ${WORKDIR} >>\033[0m"
    oe_runmake -j 1 BOARD="RAPTOR"
}

do_install_append() {
    echo "${WORKDIR}/git/out/bl1-raptor.bin" >> ${BASE_WORKDIR}/image_where.txt
#   install ${WORKDIR}/git/out/bl1-raptor.bin ${DEPLOY_DIR_IMAGE}/bl1-raptor.bin
}

