inherit linux-nexell-base

DESCRIPTION = "nx-vidtex"
LICENSE = "CLOSED"

SRC_URI = "file://lib \
          "

S = "${WORKDIR}"

PV = "1"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_install() {
    install -d ${D}${libdir}
    install -m 0644 ${S}/lib/libnxvidtex.so-${ARCH_TYPE_NUM} ${D}${libdir}/libnxvidtex.so
    install -m 0644 ${S}/lib/libnxvidtex.so-${ARCH_TYPE_NUM} ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/libnxvidtex.so
}

INSANE_SKIP_${PN} = "dev-so ldflags invalid-packageconfig"
FILES_${PN} = "${libdir}"
FILES_SOLIBSDEV = ""