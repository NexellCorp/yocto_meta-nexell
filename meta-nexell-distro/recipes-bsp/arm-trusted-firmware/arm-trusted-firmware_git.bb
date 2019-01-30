DESCRIPTION = "ATF for s5p6818"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://license.md;md5=829bdeb34c1d9044f393d5a16c068371"
PV ?= "1.0+git${SRCPV}"
SRCREV = "${AUTOREV}"

SRC_URI = "git://review.gerrithub.io/NexellCorp/secure_arm-trusted-firmware;protocol=https;branch=nexell"

S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
    :
}

#addtask mypatch after do_unpack before do_patch