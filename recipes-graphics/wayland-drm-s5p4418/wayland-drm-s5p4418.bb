DESCRIPTION = "waylanddrm-dma_buf libraries"
LICENSE = "CLOSED"
SECTION = "libs"

SRC_URI += " \
    file://bin \
    file://lib \
  "

S = "${WORKDIR}"

COMPATIBLE_MACHINE = "(s5p4418-navi-ref|s5p4418-avn-ref)"
PACKAGE_ARCH = "${MACHINE_ARCH}"

do_install () {
    install -d ${D}${bindir}
    install -d ${D}${libdir}/driver
    
    install -m 0644 ${S}/bin/* ${D}${bindir}
    install -m 0644 ${S}/lib/* ${D}${libdir}/driver
}

INSANE_SKIP_${PN} = "already-stripped debug-files"
PACKAGES = "${PN} libMali.so"
FILES_${PN} += "${libdir}/driver ${bindir}"
#ALLOW_EMPTY_${PN} = "1"