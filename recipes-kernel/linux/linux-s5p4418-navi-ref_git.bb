FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LICENSE = "GPLv2"
LINUX_VERSION ?= "4.1"

SRCREV = "0f46f08ac94418855bcb1736ee4d590d1b800807"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7" 

SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.1.15;protocol=git;branch=nexell \
	   file://0001-Yocto-navi-ref-defconfig-changed-for-QT-working.patch \
           file://0001-embedded-mali400-remove.patch \
          "

require linux-s5p4418-navi-ref.inc