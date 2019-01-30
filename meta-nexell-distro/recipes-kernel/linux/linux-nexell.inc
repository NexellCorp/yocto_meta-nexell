DESCRIPTION ?= "Linux Kernel"
SECTION = "kernel"
LICENSE = "GPLv2"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LINUX_VERSION ?= "4.4.83"

PV = "${LINUX_VERSION}+git${SRCPV}"
SRCREV = "${AUTOREV}"

SRC_URI = "git://review.gerrithub.io/NexellCorp/linux_kernel-4.4.x;protocol=https;branch=nexell;depth=5"

LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"
PROVIDES += "virtual/kernel kernel kernel-base kernel-image kernel-dev kernel-vmlinux kernel-misc kernel-modules perf-dbg perf"

S = "${WORKDIR}/git"

#inherit kernel-yocto
require recipes-kernel/linux/linux-yocto.inc
#require recipes-kernel/linux/linux-dtb.inc
#require recipes-kernel/linux/kernel-devsrc.bb

do_configure_prepend() {
    rm ${B}/.config
}

do_kernelpostprocess() {
    echo "${KBUILD_OUTPUT}" > ${BASE_WORKDIR}/KBUILD_OUTPUT_PATH.txt
    echo "${KBUILD_DEFCONFIG}" > ${BASE_WORKDIR}/KBUILD_DEFCONFIG.txt
    echo "${KERNEL_DEVICETREE}" > ${BASE_WORKDIR}/KBUILD_DEVICETREE.txt
}
