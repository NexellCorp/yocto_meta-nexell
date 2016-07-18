FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LICENSE = "GPLv2"
LINUX_VERSION ?= "4.1"

SRCREV = "41ffe83aa952b86ff5adca6f75c8892aede3a07a"
#2a0121dbc7c6c93ce9bfde6edfba8a8ff0e238cf"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7" 

SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.1.15;protocol=git;branch=nexell \
           file://0001-Yocto-avn-ref-defconfig-changed-for-QT-working.patch \
	   file://0001-Yocto-dts-nx_drm-file-changed-for-mali-working-2.patch \
          "

require linux-s5p4418-avn-ref.inc