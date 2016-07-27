#SECTION = "bootloader"
LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://README;md5=754608f69d5791d96a0a96281ae48814"
SRCREV = "3e32543bc3cbdac6960b72712005b18a0210d0c8"
SRC_URI = "git://git.nexell.co.kr/nexell/bl1/bl1-s5p6818;protocol=git;branch=artik"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"
COMPATIBLE_MACHINE = "s5p6818-artik710-raptor"

TOOLCHAIN_ARCH64_NONE_ELF = "${BASE_WORKDIR}/gcc-linaro-aarch64-none-elf-4.8-2014.04_linux/bin/aarch64-none-elf-"
TOOLCHAIN_ARCH32_EABI = "${BASE_WORKDIR}/arm-eabi-4.8/bin/"

EXTRA_OEMAKE = "\
    'VPATH=${WORKDIR}/git' \
"

do_compile () {
    oe_runmake CROSS_TOOL_TOP=${TOOLCHAIN_ARCH32_EABI} BOARD="RAPTOR" -j 1
}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/out/bl1-raptor.bin ${DEPLOY_DIR_IMAGE}
}

addtask deploy after do_install