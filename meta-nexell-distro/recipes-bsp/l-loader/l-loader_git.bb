DESCRIPTION = "l-loader for s5p6818"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://COPYING;md5=e8c1458438ead3c34974bc0be3a03ed6"

PV ?= "1.0+git${SRCPV}"
SRCREV = "${AUTOREV}"

SRC_URI = "git://review.gerrithub.io/NexellCorp/secure_l-loader;protocol=https;branch=nexell"

S = "${WORKDIR}/git"

DEFAULT_PREFERENCE = "1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
    :
}

#addtask mypatch after do_unpack before do_patch