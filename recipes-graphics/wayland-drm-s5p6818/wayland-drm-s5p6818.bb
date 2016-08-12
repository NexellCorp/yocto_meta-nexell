DESCRIPTION = "waylanddrm-dma_buf libraries"
LICENSE = "CLOSED"
SECTION = "libs"

SRC_URI += " \
    file://bin \
    file://lib \
  "

S = "${WORKDIR}"

COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p6818-avn-ref)"
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
