DESCRIPTION ?= "Linux Kernel"
SECTION = "kernel"
LICENSE = "GPLv2"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

LICENSE = "GPLv2"
LINUX_VERSION ?= "4.4.19"

SRC_URI[md5sum] = "dbb4a2b9d337a90a304d3e30b86b8d27"
SRC_URI[sha256sum] = "421ddaca0fa29bf8be2affea6fb020ab5bcfb67f368d8f7bba8d8cfd8c88a61f"

SRCREV = "9db2c167307268e6423472662ef8611c85079ac4"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/kernel/kernel-4.1.15;protocol=git;branch=nexell"

LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

PE = "1"
PV = "${LINUX_VERSION}"

inherit kernel siteinfo
require recipes-kernel/linux/linux-yocto.inc

do_configure_prepend() {
    # Clean .config
    echo "" > ${B}/.config
    CONF_SED_SCRIPT=""

    # Set cmdline
    # kernel_configure_variable CMDLINE "\"${CMDLINE}\""

    # Keep this the last line
    # Remove all modified configs and add the rest to .config
    sed -e "${CONF_SED_SCRIPT}" < '${WORKDIR}/defconfig' >> '${B}/.config'

    yes '' | oe_runmake oldconfig
}

# Automatically depend on lzop-native if CONFIG_KERNEL_LZO is enabled
python () {
     try:
         defconfig = bb.fetch2.localpath('file://defconfig', d)
     except bb.fetch2.FetchError:
         return

     try:
         configfile = open(defconfig)
     except IOError:
         return

     if 'CONFIG_KERNEL_LZO=y\n' in configfile.readlines():
         depends = d.getVar('DEPENDS', False)
         d.setVar('DEPENDS', depends + ' lzop-native')
}