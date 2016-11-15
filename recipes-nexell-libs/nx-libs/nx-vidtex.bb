DESCRIPTION = "nx-vidtex"
LICENSE = "CLOSED"

SRC_URI = "file://libnxvidtex.so \
          "

S = "${WORKDIR}"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_install() {
    install -d ${D}${libdir}
    install -m 0644 libnxvidtex.so ${D}${libdir}
}

INSANE_SKIP_${PN} = "dev-so ldflags"
FILES_${PN} = "${libdir}"
FILES_SOLIBSDEV = ""