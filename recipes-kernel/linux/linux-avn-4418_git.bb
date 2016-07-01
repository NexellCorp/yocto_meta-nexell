FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LICENSE = "GPLv2"
LINUX_VERSION ?= "4.1"

SRCREV = "2a0121dbc7c6c93ce9bfde6edfba8a8ff0e238cf"
#9734aadeb02c102362e17926af2f23c04a806181"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7" 

SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.1.15;protocol=git;branch=nexell \
           file://0001-defconfig-modified.patch "

require linux-avn-4418.inc