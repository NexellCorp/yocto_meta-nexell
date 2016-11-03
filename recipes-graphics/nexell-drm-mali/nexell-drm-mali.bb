inherit linux-nexell-base

DESCRIPTION = "waylanddrm-dma_buf libraries"
LICENSE = "CLOSED"
SECTION = "libs"

TYPE = "mali-nexell"

SRC_URI += "file://bin \
            file://lib \
           "

S = "${WORKDIR}"

DEPENDS += "${@base_contains("DISTRO_FEATURES", "wayland", " mesa", " ", d)}"
PROVIDES += "virtual/egl virtual/libgles1 virtual/libgles2 virtual/libopencl virtual/libwayland-egl"

PACKAGE_ARCH = "${MACHINE_ARCH}"
ARCH_TYPE_NUM = "${@get_kernel_arch_num(d,"${TARGET_PREFIX}")}"

do_install () {
    install -d ${D}${bindir}
    install -d ${D}${libdir}

    install -m 0755 ${S}/bin/egl_api_main_suite_20-${ARCH_TYPE_NUM} ${D}${bindir}/egl_api_main_suite_20
    install -m 0755 ${S}/bin/gles2_api_suite-${ARCH_TYPE_NUM} ${D}${bindir}/gles2_api_suite
    install -m 0644 ${S}/lib/libMali.so-${ARCH_TYPE_NUM} ${D}${libdir}/libMali.so

    # install -m 0644 ${S}/bin/* ${D}${bindir}
    # install -m 0644 ${S}/lib/* ${D}${libdir}

    # Create MALI manifest
    if [ "${ARCH_TYPE_NUM}" -eq "64" ]; then
        ln ${D}${libdir}/libMali.so ${D}${libdir}/libEGL.so
        ln ${D}${libdir}/libMali.so ${D}${libdir}/libGLESv1_CM.so
        ln ${D}${libdir}/libMali.so ${D}${libdir}/libGLESv2.so
        ln ${D}${libdir}/libMali.so ${D}${libdir}/libOpenCL.so
        ln ${D}${libdir}/libMali.so ${D}${libdir}/libgbm.so
        ln ${D}${libdir}/libMali.so ${D}${libdir}/libwayland-egl.so
    else
        ln -sf ${D}${libdir}/libMali.so ${D}${libdir}/libEGL.so
        ln -sf ${D}${libdir}/libMali.so ${D}${libdir}/libGLESv1_CM.so
        ln -sf ${D}${libdir}/libMali.so ${D}${libdir}/libGLESv2.so
        ln -sf ${D}${libdir}/libMali.so ${D}${libdir}/libOpenCL.so
        ln -sf ${D}${libdir}/libMali.so ${D}${libdir}/libgbm.so
        ln -sf ${D}${libdir}/libMali.so ${D}${libdir}/libwayland-egl.so
    fi
}

INSANE_SKIP_${PN} = "already-stripped debug-files dev-so ldflags"
PACKAGES = "${PN}"
FILES_${PN} += "${bindir} ${libdir}"

RREPLACES_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libgbm"
RPROVIDES_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libgbm"
RPROVIDES_${PN} += " libGLESv2.so libEGL.so libgbm.so libwayland-egl.so libGLESv1_CM.so"
RCONFLICTS_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libgbm"