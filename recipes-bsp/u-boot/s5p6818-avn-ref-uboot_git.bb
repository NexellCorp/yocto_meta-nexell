vDESCRIPTION = "U-Boot for avn-ref"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://README;md5=587d8d939ae351bb11910921b1eafd42"

SRCREV = "cb3dd133e2d07166e9ceb30da86e17880059ad9b"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/u-boot/u-boot-2016.01;protocol=git;branch=nexell \
          "

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "s5p6818-avn-ref"

do_compile() {
    local config="s5p6818_arm64_avn_ref"

    oe_runmake clean
    oe_runmake ${config}_defconfig
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