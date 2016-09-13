FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LICENSE = "GPLv2"
LINUX_VERSION ?= "4.1"

SRCREV = "69e397b31faff4ba0e636b6e96e28efec851ddb6"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7" 

SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.1.15;protocol=git;branch=yocto-navi \
		file://0001-Yocto-mali400-Kbuild-compile-error-fix.patch \
          "

require linux-s5p4418-navi-ref.inc
