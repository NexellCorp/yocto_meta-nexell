FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LICENSE = "GPLv2"
LINUX_VERSION ?= "4.1"

SRCREV = "d5ca1a2efb0ba3b628b14c6eceea64f9641f9c4a"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7" 

SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.1.15;protocol=git;branch=nexell \
	   file://0001-Yocto-navi-ref-defconfig-changed-for-QT-working.patch \
           file://0001-Yocto-dts-nx_drm-file-changed-for-mali-working-2.patch \
          "

require linux-s5p4418-navi-ref.inc