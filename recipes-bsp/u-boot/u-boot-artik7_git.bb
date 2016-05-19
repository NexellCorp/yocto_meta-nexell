DESCRIPTION = "U-Boot for artik7"

#DEPENDS += "optee-build"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://README;md5=587d8d939ae351bb11910921b1eafd42"

SRCREV = "6cb05dd0f0e1eb9abf46e068448720acecde0ce9"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/u-boot/u-boot-2016.01;protocol=git;branch=artik"

S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "artik710-raptor"


#do_deploy_append() {
#    cp -a ${WORKDIR}/git/u-boot.bin ${DEPLOY_DIR_IMAGE}/u-boot.bin
#}

do_compile() {
    local config="artik710_raptor"
    local sdram_size=

    oe_runmake clean
    oe_runmake ${config}_config
    oe_runmake CROSS_COMPILE=aarch64-poky-linux- LDFLAGS="" CC="$CC"
}

do_install_append() {
    echo "${WORKDIR}/git/u-boot.bin" >> ${BASE_WORKDIR}/image_where.txt
#    install ${WORKDIR}/git/u-boot.bin ${DEPLOY_DIR_IMAGE}/u-boot.bin
}
