DESCRIPTION = "U-Boot for artik710-raptor"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://README;md5=587d8d939ae351bb11910921b1eafd42"

SRCREV = "98d1731dc354a6ecd161ebb74e3fda8c92bccb79"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/u-boot/u-boot-2016.01;protocol=git;branch=nexell"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "s5p6818-artik710-raptor"

do_compile() {
    local config="artik710_raptor"

    oe_runmake clean
    oe_runmake ${config}_config
    oe_runmake CROSS_COMPILE=aarch64-poky-linux- LDFLAGS="" CC="$CC"
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
