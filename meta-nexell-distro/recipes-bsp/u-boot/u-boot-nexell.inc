LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://README;md5=587d8d939ae351bb11910921b1eafd42"

SRCREV = "56060e157bb0e9e28ce4d890c04e172d7205d2dc"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/u-boot/u-boot-2016.01;protocol=git;branch=nexell"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PROVIDES += "u-boot"

PACKAGE_ARCH = "${MACHINE_ARCH}"
