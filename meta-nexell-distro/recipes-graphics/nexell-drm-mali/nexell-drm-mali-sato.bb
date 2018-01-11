inherit linux-nexell-base

DESCRIPTION = "x11 drm-dma_buf libraries"
LICENSE = "CLOSED"
SECTION = "libs"

TYPE = "mali-nexell"

SRC_URI = "file://library \
           file://sato/egl.pc \
           file://sato/include \
           "

S = "${WORKDIR}"

PROVIDES = " virtual/libgles1 virtual/libgles2 "
PROVIDES += "${@bb.utils.contains("DISTRO_FEATURES", "x11", " ", " virtual/egl", d)}"
DEPENDS = "libdrm libxau libxext libx11 libxfixes libxdamage libxdmcp libxcb"

PACKAGE_ARCH = "${MACHINE_ARCH}"

#sato? or qt
CURRENT_IMAGE_TYPE = "${@get_image_type(d,"${DEPLOY_DIR}","${MACHINE_ARCH}")}"

do_install () {
    install -d ${D}${libdir}
    install -d ${D}${includedir}
    install -d ${D}${libdir}/nexell

    install -m 0644 ${S}/library/${ARCH_TYPE_NUM}/*.so ${D}${libdir}/nexell/

    cd ${D}${libdir}
    ln -sf libMali.so libEGL.so
    ln -sf libMali.so libGLESv1_CM.so
    ln -sf libMali.so libGLESv2.so
    ln -sf libMali.so libwayland-egl.so

    # install headers
    # install -d -m 0755                           ${D}${includedir}/EGL
    # install -m 0755 ${S}/sato/include/EGL/*.h    ${D}${includedir}/EGL/
    # install -d -m 0755                           ${D}${includedir}/GLES
    # install -m 0755 ${S}/sato/include/GLES/*.h   ${D}${includedir}/GLES/
    # install -d -m 0755                           ${D}${includedir}/GLES2
    # install -m 0755 ${S}/sato/include/GLES2/*.h  ${D}${includedir}/GLES2/
    # install -d -m 0755                           ${D}${includedir}/KHR
    # install -m 0755 ${S}/sato/include/KHR/*.h    ${D}${includedir}/KHR/

    # Copy the .pc files
    # install -d -m 0755                                 ${D}${libdir}/pkgconfig
    # install -m 0644  ${WORKDIR}/sato/*.pc              ${D}${libdir}/pkgconfig/
}

INSANE_SKIP_${PN} = "already-stripped debug-files dev-so ldflags"
PACKAGES = "${PN}"

FILES_${PN} = "${libdir} ${includedir}"
FILES_${PN}-dev = "${includedir} ${libdir}/pkgconfig/*"

RREPLACES_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2"
RPROVIDES_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2"
RPROVIDES_${PN} += "libGLESv2.so libEGL.so libGLESv2.so libGLESv1_CM.so libMali.so"
RCONFLICTS_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2"
