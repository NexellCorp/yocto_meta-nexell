#SECTION = "bootloader"
LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://README;md5=c48cff868868eb40f73e510f7bae7710"
SRCREV = "b9de286dcb857b546d738be32a0f37b96ef0b52f"
SRC_URI = "git://git.nexell.co.kr/nexell/bl1/bl1-s5p4418;protocol=git;branch=artik \
           file://0001-bl1-AVN.patch \
          "

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"
COMPATIBLE_MACHINE = "avn-4418"

TOOLCHAIN_ARCH32_EABI = "${BASE_WORKDIR}/arm-eabi-4.8/bin/"

EXTRA_OEMAKE = "\
    'VPATH=${WORKDIR}/git' \
"

do_compile () {
    oe_runmake CROSS_TOOL_TOP=${TOOLCHAIN_ARCH32_EABI} BOARD="AVN" -j 1
}

do_install_append() {
    echo "${WORKDIR}/git/out/bl1-avn.bin" >> ${BASE_WORKDIR}/image_where.txt
}
