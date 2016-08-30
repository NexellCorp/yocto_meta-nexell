FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LICENSE = "GPLv2"
LINUX_VERSION ?= "4.1"

SRCREV = "442f61d2c56837cea9c9905f0ba9a0edbc11cffb"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7" 

SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.1.15;protocol=git;branch=nexell \
	   file://0001-Yocto-navi-ref-defconfig-changed-for-QT-working.patch \
           file://0001-Yocto-mali400-Kbuild-compile-error-fix.patch \
           file://0001-navi-4418-temporary.patch \
          "

require linux-s5p4418-navi-ref.inc