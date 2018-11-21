DESCRIPTION = "gcc-linaro aarch64 external toolchain"
LICENSE="GPLv2"

LIC_FILES_CHKSUM = "file://README;md5=c1d3bc4a104cf57c0cd5575e73a3e3a4"

SRCREV="571d7ad58d412ee871b1760747609160b202df17"

SRC = "gcc-linaro-4.9-2015.05-x86_64_aarch64-linux-gnu.tar.gz"

SRC_URI = "git://github.com/NexellCorp/external_toolchain;protocol=git"

SRC_URI[md5sum] = "afc1981b69a5c30e3eb87f8198e5dcf2"
SRC_URI[sha256sum] = "c7a66128a0926a512c7c47df461ecc04f72ded96a973a09c15171d8b498793ed"

S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_configure() {
    :
}
do_compile() {
    :
}
do_install () {
    install -d ${D}${datadir}/${TOOLCHAIN_AARCH64_PREBUILT}

    cat ${SRC}* | tar -xvzf - -C ${D}${datadir}/${TOOLCHAIN_AARCH64_PREBUILT}
}

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_SYSROOT_STRIP = "1"

INSANE_SKIP_${PN} = "installed-vs-shipped already-stripped license-checksum"
INSANE_SKIP_${PN}-dbg = "installed-vs-shipped already-stripped"
