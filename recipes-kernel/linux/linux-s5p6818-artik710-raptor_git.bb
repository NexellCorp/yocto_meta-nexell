FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LICENSE = "GPLv2"
LINUX_VERSION ?= "4.1"

SRCREV = "71f0ea185af641143b6afea8f8d43ddbde210da1"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7" 

SRC_URI[md5sum] = "dbb4a2b9d337a90a304d3e30b86b8d27"
SRC_URI[sha256sum] = "421ddaca0fa29bf8be2affea6fb020ab5bcfb67f368d8f7bba8d8cfd8c88a61f"

SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.1.15;protocol=git;branch=artik; \
           file://0001-arm64-dts-s5p6818-add-mali400-utgard-dt-node.patch; \
           file://0001-drm-nexell-add-nx_drm_gem_dumb_create-for-pitch-alig.patch \
          "

require linux-s5p6818-artik710-raptor.inc