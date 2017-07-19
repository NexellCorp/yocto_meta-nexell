DESCRIPTION = "gcc-linaro external toolchain"
LICENSE="GPLv2"

LIC_FILES_CHKSUM = "file://share/doc/standards.html;md5=0bf5b1722feaf023aa1b23dc36c2bf2c"

ARM_ABI = "arm-linux-gnueabihf"
VERSION = "4.9-2014.11-x86_64"

SRC_URI = "https://releases.linaro.org/archive/14.11/components/toolchain/binaries/${ARM_ABI}/gcc-linaro-${VERSION}_${ARM_ABI}.tar.xz"
SRC_URI[md5sum] = "6cacdc0c896bae5d7c6f074e2e219db9"
SRC_URI[sha256sum] = "bc4ca2ced084d2dc12424815a4442e19cb1422db87068830305d90075feb1a3b"

S = "${WORKDIR}/gcc-linaro-${VERSION}_${ARM_ABI}"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_install () {
    install -d ${D}${datadir}/gcc-linaro-4.9-${ARM_ABI}
    cp -r ${S}/. ${D}${datadir}/gcc-linaro-4.9-${ARM_ABI}
}

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_SYSROOT_STRIP = "1"

INSANE_SKIP_${PN} = "installed-vs-shipped already-stripped"
INSANE_SKIP_${PN}-dbg = "installed-vs-shipped already-stripped"
