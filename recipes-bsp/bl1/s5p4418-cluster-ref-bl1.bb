#SECTION = "bootloader"
LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://README;md5=c48cff868868eb40f73e510f7bae7710"
SRCREV = "1d2a94728488125a5c9346f7d347f10d4f01cca5"
SRC_URI = "git://git.nexell.co.kr/nexell/bl1/bl1-s5p4418;protocol=git;branch=yocto-dmc \
           file://0001-NAVI-NSP4330-Bl1-Board-SETTING.patch \
          "

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"
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
