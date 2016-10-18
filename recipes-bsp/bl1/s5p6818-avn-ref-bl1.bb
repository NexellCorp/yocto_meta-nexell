include bl1-secure.inc

LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://README;md5=754608f69d5791d96a0a96281ae48814"
SRCREV = "976cf325fef42aecd967d771b6a9bb7525a750a8"
SRC_URI = "git://git.nexell.co.kr/nexell/bl1/bl1-s5p6818;protocol=git;branch=nexell \
           file://0001-s5p6818-avn-bl1.patch \
          "
	
S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"
COMPATIBLE_MACHINE = "s5p6818-avn-ref"

TOOLCHAIN_ARCH64_NONE_ELF = "${BASE_WORKDIR}/gcc-linaro-aarch64-none-elf-4.8-2014.04_linux/bin/"
TOOLCHAIN_ARCH32_EABI = "${BASE_WORKDIR}/arm-eabi-4.8/bin/"

EXTRA_OEMAKE = "\
    'VPATH=${WORKDIR}/git' \
"

do_compile () {
    oe_runmake CROSS_TOOL_TOP=${TOOLCHAIN_ARCH32_EABI} BOARD="AVN" ${SECURE-BL1} -j 1
}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/out/bl1-avn.bin ${DEPLOY_DIR_IMAGE}
}

addtask deploy after do_install