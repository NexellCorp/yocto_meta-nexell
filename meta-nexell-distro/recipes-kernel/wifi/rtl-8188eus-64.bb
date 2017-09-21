SUMMARY = "Copy wlan.ko to image deployment area"
SECTION = "devel"
LICENSE = "GPLv2"
SRC_URI = "file://rtl8188EUS_linux_v4.3.0.8_13968.20150417_patch_to_kernel_4.1.tar.gz"

LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

INHIBIT_PACKAGE_STRIP="1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

S = "${WORKDIR}"

DEPENDS = "virtual/kernel"

inherit module

EXTRA_OEMAKE += 'ARCH=arm64 KSRC="${STAGING_KERNEL_DIR}" LDFLAGS="-O1 --hash-style=gnu --as-needed"'

do_install () {
   install -d ${D}${bindir}
   install -d ${BASE_WORKDIR}/extra-rootfs-support/usr/bin/

   install -m 0755 ${WORKDIR}/wlan.ko ${D}${bindir}  #images/usr/bin
   install -m 0755 ${WORKDIR}/wlan.ko ${BASE_WORKDIR}/extra-rootfs-support/usr/bin/
}

FILES_${PN} = "${bindir}"
ALLOW_EMPTY_${PN} = "1"
