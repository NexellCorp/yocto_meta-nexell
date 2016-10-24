FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LICENSE = "GPLv2"

LINUX_VERSION ?= "4.4.19"
NEXELL_BRANCH = "nexell"

SRCREV = "563e55a644651ff13f8b1300e587f0ead789ac23"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7" 

SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.1.15;protocol=git;branch=nexell \
           file://0001-Yocto-mali400-Kbuild-compile-error-fix.patch \
          "

require linux-s5p4418-hs-iot.inc
