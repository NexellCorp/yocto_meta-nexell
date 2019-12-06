inherit linux-nexell-base

DESCRIPTION = "waylanddrm-dma_buf libraries"
LICENSE = "CLOSED"
SECTION = "libs"

TYPE = "mali-nexell"

SRC_URI = "file://src \
           file://library \
           "

S = "${WORKDIR}"

DEPENDS += "${@bb.utils.contains("DISTRO_FEATURES", "nexell-mali-qt", " mesa", " ", d)} \
			patchelf-native \
			"
PROVIDES += "virtual/egl virtual/libgles1 virtual/libgles2 virtual/libopencl virtual/libwayland-egl"

PACKAGE_ARCH = "${MACHINE_ARCH}"

CURRENT_IMAGE_TYPE = "${@get_image_type(d,"${DEPLOY_DIR}","${MACHINE_ARCH}")}"

do_install () {
    install -d ${D}${bindir}
    install -d ${D}${libdir}
    install -d ${D}${libdir}/nexell

    install -m 0755 ${S}/src/bin/egl_api_main_suite_20-${ARCH_TYPE_NUM} ${D}${bindir}/egl_api_main_suite_20
    install -m 0755 ${S}/src/bin/gles2_api_suite-${ARCH_TYPE_NUM} ${D}${bindir}/gles2_api_suite

    install -m 0644 ${S}/library/${ARCH_TYPE_NUM}/*.so ${D}${libdir}/nexell/
    install -m 0755 ${S}/library/change32LibMaliToFB.sh ${D}${bindir}
    install -m 0755 ${S}/library/change32LibMaliToWAYLAND.sh ${D}${bindir}

	patchelf --set-soname ${D}${libdir}/nexell/libMali-${ARCH_TYPE_NUM}-WAYLAND.so ${D}${libdir}/nexell/libMali-${ARCH_TYPE_NUM}-WAYLAND.so

    ln ${D}${libdir}/nexell/libMali-${ARCH_TYPE_NUM}-WAYLAND.so ${D}${libdir}/libMali.so


    # Create MALI manifest
    cd ${D}${libdir}

	patchelf --set-soname libMali.so libMali.so


    if [ "${ARCH_TYPE_NUM}" -eq "64" ];then
        ln -f libMali.so libEGL.so
        ln -f libMali.so libGLESv1_CM.so
        ln -f libMali.so libGLESv2.so
        ln -f libMali.so libOpenCL.so
        ln -f libMali.so libgbm.so
        #ln -f libMali.so libwayland-egl.so
    else
        ln -sf libMali.so libEGL.so
        ln -sf libMali.so libGLESv1_CM.so
        ln -sf libMali.so libGLESv2.so
        ln -sf libMali.so libOpenCL.so
        ln -sf libMali.so libgbm.so
        #ln -sf libMali.so libwayland-egl.so
    fi
}

INSANE_SKIP_${PN} = "already-stripped debug-files dev-so ldflags file-rdeps"
PACKAGES = "${PN}"
FILES_${PN} += "${bindir} ${libdir} ${libdir}/nexell ${includedir}/KHR"

RREPLACES_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libgbm"
RPROVIDES_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libgbm"
RPROVIDES_${PN} += " libGLESv2.so libEGL.so libgbm.so libwayland-egl.so libGLESv1_CM.so"
RCONFLICTS_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libgbm"
