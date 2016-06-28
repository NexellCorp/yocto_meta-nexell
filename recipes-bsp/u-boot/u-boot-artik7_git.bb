DESCRIPTION = "U-Boot for artik710-raptor"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://README;md5=587d8d939ae351bb11910921b1eafd42"

SRCREV = "3d4ce985752093914607424f967bca558df74fab"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/u-boot/u-boot-2016.01;protocol=git;branch=artik"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "artik710-raptor"

do_compile() {
    local config="artik710_raptor"

    oe_runmake clean
    oe_runmake ${config}_config
    oe_runmake CROSS_COMPILE=aarch64-poky-linux- LDFLAGS="" CC="$CC"
}

do_install_append() {
    echo "${WORKDIR}/git/u-boot.bin" >> ${BASE_WORKDIR}/image_where.txt
}

PROVIDES = "u-boot-artik7"