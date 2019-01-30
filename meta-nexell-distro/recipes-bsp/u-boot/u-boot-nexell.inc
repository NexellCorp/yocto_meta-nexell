LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://README;md5=587d8d939ae351bb11910921b1eafd42"

PV ?= "1.0+git${SRCPV}"
SRCREV = "${AUTOREV}"

SRC_URI = "git://review.gerrithub.io/NexellCorp/u-boot-2016.01;protocol=https;branch=nexell"

S = "${WORKDIR}/git"

PROVIDES += "u-boot"

PACKAGE_ARCH = "${MACHINE_ARCH}"
