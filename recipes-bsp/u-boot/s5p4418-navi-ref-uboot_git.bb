DESCRIPTION = "U-Boot for AVN S5P4418"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://README;md5=587d8d939ae351bb11910921b1eafd42"

SRCREV = "3ea669cfaa8a9ea212aba9b68671832b01cadaf0"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/u-boot/u-boot-2016.01;protocol=git;branch=nexell \
           "

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "s5p4418-navi-ref"

TOOLCHAIN_CORTEXA9 = "${BASE_WORKDIR}/arm-cortex_a9-eabi-4.7-eglibc-2.18"
TOOLCHAIN_CORTEXA9_BIN = "${BASE_WORKDIR}/arm-cortex_a9-eabi-4.7-eglibc-2.18/bin/"

EXTRA_OEMAKE = "\
    'VPATH=${TOOLCHAIN_CORTEXA9}' \
"

do_compile() {
    oe_runmake clean
    oe_runmake ARCH=arm s5p4418_navi_ref_defconfig
    oe_runmake ARCH=arm CROSS_COMPILE="${TOOLCHAIN_CORTEXA9_BIN}arm-cortex_a9-linux-gnueabi-"
}

do_install() {
    cp `find . -name "env_common.o"` copy_env_common.o
    ${TARGET_PREFIX}objcopy -O binary --only-section=.rodata.default_environment `find . -name "copy_env_common.o"`
    tr '\0' '\n' < copy_env_common.o > default_envs.txt
    cp default_envs.txt default_envs.txt.orig
}

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/u-boot.bin ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/default_envs.txt ${DEPLOY_DIR_IMAGE}
}

addtask deploy after do_install