FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

DEPENDS += "bl1-artik7"
DEPENDS += "u-boot-artik7"

LINUX_VERSION ?= "4.1"

SRCREV = "de3c20ee5f02e990000df04fc2938e4beca88970"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7" 

SRC_URI[md5sum] = "dbb4a2b9d337a90a304d3e30b86b8d27"
SRC_URI[sha256sum] = "421ddaca0fa29bf8be2affea6fb020ab5bcfb67f368d8f7bba8d8cfd8c88a61f"

SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.1.15;protocol=git;branch=artik"
#"git://git@bitbucket.org/kchhero/linux.git;protocol=ssh"

require linux-artik710-raptor.inc