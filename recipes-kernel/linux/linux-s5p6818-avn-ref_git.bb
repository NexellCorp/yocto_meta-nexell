FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LICENSE = "GPLv2"
LINUX_VERSION ?= "4.4.19"

SRCREV = "e908e2715d84633ab189519ebb55903af975a6ff"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7" 

SRC_URI[md5sum] = "dbb4a2b9d337a90a304d3e30b86b8d27"
SRC_URI[sha256sum] = "421ddaca0fa29bf8be2affea6fb020ab5bcfb67f368d8f7bba8d8cfd8c88a61f"

SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.1.15;protocol=git;branch=nexell; \
           file://0001-Yocto-mali400-Kbuild-compile-error-fix.patch \
          "

require linux-s5p6818-avn-ref.inc