DESCRIPTION = "U-Boot for AVN S5P4418"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://README;md5=587d8d939ae351bb11910921b1eafd42"

SRCREV = "28b408e63d55d44b8b77e54fcc586f784ab0cd1b"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/u-boot/u-boot-2016.01;protocol=git;branch=nexell \
           file://0001-bootcmd-add-for-avn-yocto.patch"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "avn"

do_compile() {
    oe_runmake clean
    oe_runmake s5p4418_avn_ref_config
    oe_runmake LDFLAGS="" CC="$CC"
}

do_install_append() {
    echo "${WORKDIR}/git/u-boot.bin" >> ${BASE_WORKDIR}/image_where.txt
}

PROVIDES = "u-boot-avn"