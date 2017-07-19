DESCRIPTION = "arm-eabi-4.8 external toolchain"
SUMMARY = "arm-eabi-4.8"
LICENSE="GPLv2 & GPLv3 & LGPLv2 & LGPLv3"

LIC_FILES_CHKSUM = "file://SOURCES;md5=54a4875c3bc190abaa44463a307e43df"

SRCREV = "26e93f6af47f7bd3a9beb5c102a5f45e19bfa38a"
SRC_URI = "git://android.googlesource.com/platform/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8;protocol=https"

S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_install () {
    install -d ${D}${datadir}/arm-eabi-4.8-toolchain
    cp -r ${S}/. ${D}${datadir}/arm-eabi-4.8-toolchain/
}

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_SYSROOT_STRIP = "1"

INSANE_SKIP_${PN} = "installed-vs-shipped already-stripped"
INSANE_SKIP_${PN}-dbg = "installed-vs-shipped already-stripped"
