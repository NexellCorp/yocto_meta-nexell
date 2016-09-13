FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LICENSE = "GPLv2"
LINUX_VERSION ?= "4.1"

SRCREV = "5d6fcc764bfcc4d8241084e9cced923dcf6be709"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7" 

SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.1.15;protocol=git;branch=yocto-dmc \
		file://0001-Yocto-mali400-Kbuild-compile-error-fix.patch \
          "

require linux-s5p4418-cluster-ref.inc
