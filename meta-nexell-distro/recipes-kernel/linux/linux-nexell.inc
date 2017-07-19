DESCRIPTION ?= "Linux Kernel"
SECTION = "kernel"
LICENSE = "GPLv2"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LINUX_VERSION ?= "4.4.49"

SRCREV = "dda3834a34289130367b4402789e98f94e0e9457"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.4.x;protocol=git;branch=nexell"

LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"
PROVIDES += "virtual/kernel kernel kernel-base kernel-image kernel-dev kernel-vmlinux kernel-misc kernel-modules perf-dbg perf"

PE = "1"
PV = "${LINUX_VERSION}"
S = "${WORKDIR}/git"

#inherit kernel-yocto
require recipes-kernel/linux/linux-yocto.inc
#require recipes-kernel/linux/linux-dtb.inc
#require recipes-kernel/linux/kernel-devsrc.bb

do_configure_prepend() {
    rm ${B}/.config
}

# # Automatically depend on lzop-native if CONFIG_KERNEL_LZO is enabled
# python () {
#      try:
#          defconfig = bb.fetch2.localpath('file://defconfig', d)
#      except bb.fetch2.FetchError:
#          return

#      try:
#          configfile = open(defconfig)
#      except IOError:
#          return

#      if 'CONFIG_KERNEL_LZO=y\n' in configfile.readlines():
#          depends = d.getVar('DEPENDS', False)
#          d.setVar('DEPENDS', depends + ' lzop-native')
# }