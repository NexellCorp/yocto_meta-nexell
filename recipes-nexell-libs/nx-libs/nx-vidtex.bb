inherit linux-nexell-base

DESCRIPTION = "nx-vidtex"
LICENSE = "CLOSED"

SRC_URI = "file://lib \
          "

S = "${WORKDIR}"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"
ARCH_TYPE_NUM = "${@get_kernel_arch_num(d,"${TARGET_PREFIX}")}"

do_install() {
    install -d ${D}${libdir}
    install -m 0644 ${S}/lib/libnxvidtex.so-${ARCH_TYPE_NUM} ${D}${libdir}/libnxvidtex.so
}

INSANE_SKIP_${PN} = "dev-so ldflags"
FILES_${PN} = "${libdir}"
FILES_SOLIBSDEV = ""