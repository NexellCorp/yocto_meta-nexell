inherit linux-nexell-base

DESCRIPTION = "waylanddrm-dma_buf libraries"
LICENSE = "CLOSED"
SECTION = "libs"

TYPE = "mali-nexell"

SRC_URI = "file://bin \
           file://lib \
           "

S = "${WORKDIR}"

PROVIDES += "virtual/egl virtual/libgles1 virtual/libgles2 virtual/libopencl"

PACKAGE_ARCH = "${MACHINE_ARCH}"
ARCH_TYPE_NUM = "${@get_kernel_arch_num(d,"${TARGET_PREFIX}")}"
#sato? or qt
CURRENT_IMAGE_TYPE = "${@get_image_type(d,"${DEPLOY_DIR}","${MACHINE_ARCH}")}"

do_install () {
    install -d ${D}${bindir}
    install -d ${D}${libdir}

    touch ${D}${libdir}/libMali_for_X11
    install -m 0644 ${S}/lib/libMali.so-${ARCH_TYPE_NUM}-X11 ${D}${libdir}/libMali.so

    # Create MALI manifest
    if [ "${ARCH_TYPE_NUM}" -eq "64" ]; then
        ln ${D}${libdir}/libMali.so ${D}${libdir}/libEGL.so
        ln ${D}${libdir}/libMali.so ${D}${libdir}/libGLESv1_CM.so
        ln ${D}${libdir}/libMali.so ${D}${libdir}/libGLESv2.so
        ln ${D}${libdir}/libMali.so ${D}${libdir}/libOpenCL.so
        ln ${D}${libdir}/libMali.so ${D}${libdir}/libgbm.so
    else
        ln -sf libMali.so ${D}${libdir}/libEGL.so
        ln -sf libMali.so ${D}${libdir}/libGLESv1_CM.so
        ln -sf libMali.so ${D}${libdir}/libGLESv2.so
        ln -sf libMali.so ${D}${libdir}/libOpenCL.so
        ln -sf libMali.so ${D}${libdir}/libgbm.so
    fi
}

INSANE_SKIP_${PN} = "already-stripped debug-files dev-so ldflags"
PACKAGES = "${PN}"
FILES_${PN} += "${bindir} ${libdir}"

RREPLACES_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libgbm"
RPROVIDES_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libgbm"
RPROVIDES_${PN} += " libGLESv2.so libEGL.so libgbm.so libGLESv1_CM.so"
RCONFLICTS_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libgbm"
