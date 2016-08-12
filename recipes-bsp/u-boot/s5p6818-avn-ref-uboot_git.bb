vDESCRIPTION = "U-Boot for avn-ref"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://README;md5=587d8d939ae351bb11910921b1eafd42"

SRCREV = "32945754302f6e1b9ccc00d8aa14a985f7165592"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/u-boot/u-boot-2016.01;protocol=git;branch=nexell \
           file://0001-bootcmd-add-for-avn-s5p6818.patch"

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

inherit deploy

do_deploy () {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${S}/u-boot.bin ${DEPLOY_DIR_IMAGE}
}

addtask deploy after do_install