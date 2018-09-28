DESCRIPTION ?= "Linux Kernel"
SECTION = "kernel"
LICENSE = "GPLv2"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LINUX_VERSION ?= "4.4.83"

SRCREV = "0e9f91fb681d4f68e343a22a8f0e5f770f0f2583"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.4.x;protocol=git;branch=nexell;depth=5"

LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"
PROVIDES += "virtual/kernel kernel kernel-base kernel-image kernel-dev kernel-vmlinux kernel-misc kernel-modules perf-dbg perf"

PE = "1"
PV = "${LINUX_VERSION}"
S = "${WORKDIR}/git"

#inherit kernel-yocto
require recipes-kernel/linux/linux-yocto.inc
#require recipes-kernel/linux/linux-dtb.inc
#require recipes-kernel/linux/kernel-devsrc.bb

do_configure_prepend() {
    rm ${B}/.config
}
