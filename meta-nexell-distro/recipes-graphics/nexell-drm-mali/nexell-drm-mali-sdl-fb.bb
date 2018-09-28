inherit linux-nexell-base

DESCRIPTION = "waylanddrm-dma_buf libraries"
LICENSE = "CLOSED"
SECTION = "libs"

TYPE = "mali-nexell"

SRC_URI = "file://library \
           file://sdl/src_FB \
           "

S = "${WORKDIR}"

DEPENDS += "${@bb.utils.contains("DISTRO_FEATURES", "wayland", " mesa", " ", d)}"
PROVIDES += "virtual/egl virtual/libgles1 virtual/libgles2 virtual/libopencl virtual/libwayland-egl"

PACKAGE_ARCH = "${MACHINE_ARCH}"

CURRENT_IMAGE_TYPE = "${@get_image_type(d,"${DEPLOY_DIR}","${MACHINE_ARCH}")}"

do_compile() {
    cd ${S}/sdl/src_FB/ES1_tutorial1
    oe_runmake CROSSNAME=${TARGET_PREFIX} INCLUDE="-I${S}/sdl/src_FB/3rdparty/include/khronos -I${STAGING_INCDIR} -I${S}/sdl/src_FB/ES1_tutorial1"

    cd ${S}/sdl/src_FB/ES2_tutorial1
    oe_runmake CROSSNAME=${TARGET_PREFIX} INCLUDE="-I${S}/sdl/src_FB/3rdparty/include/khronos -I${STAGING_INCDIR} -I${S}/sdl/src_FB/ES2_tutorial1"
}

do_install () {
    install -d ${D}${bindir}/sdl_example
    install -d ${D}${libdir}
    install -d ${D}${libdir}/nexell

    install -m 0755 ${S}/sdl/src_FB/ES1_tutorial1/tutorial1_es1 ${D}${bindir}/sdl_example/
    install -m 0755 ${S}/sdl/src_FB/ES2_tutorial1/tutorial1_es2 ${D}${bindir}/sdl_example/
    install -m 0644 ${S}/sdl/src_FB/ES2_tutorial1/t1_rend.frag  ${D}${bindir}/sdl_example/
    install -m 0644 ${S}/sdl/src_FB/ES2_tutorial1/t1_rend.vert  ${D}${bindir}/sdl_example/

    install -m 0644 ${S}/library/${ARCH_TYPE_NUM}/*.so ${D}${libdir}/nexell/
    install -m 0755 ${S}/library/change32LibMaliToFB.sh ${D}${bindir}
    install -m 0755 ${S}/library/change32LibMaliToWAYLAND.sh ${D}${bindir}

    ln ${D}${libdir}/nexell/libMali-${ARCH_TYPE_NUM}-WAYLAND.so ${D}${libdir}/libMali.so

    cd ${D}${libdir}
    # Create MALI manifest
    ln -sf libMali.so libEGL.so
    ln -sf libMali.so libEGL.so.1
    ln -sf libMali.so libEGL.so.1.4
    ln -sf libMali.so libGLESv1_CM.so
    ln -sf libMali.so libGLESv1_CM.so.1
    ln -sf libMali.so libGLESv1_CM.so.1.1
    ln -sf libMali.so libGLESv2.so
    ln -sf libMali.so libGLESv2.so.2
    ln -sf libMali.so libGLESv2.so.2.0
    ln -sf libMali.so libwayland-egl.so
    ln -sf libMali.so libOpenCL.so
    ln -sf libMali.so libgbm.so
}

INSANE_SKIP_${PN} = "already-stripped debug-files dev-so ldflags"
PACKAGES = "${PN}"
FILES_${PN} += "${bindir}/sdl_example ${libdir}"

RREPLACES_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libgbm"
RPROVIDES_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libgbm"
RPROVIDES_${PN} += " libGLESv2.so libEGL.so libgbm.so libwayland-egl.so libGLESv1_CM.so"
RCONFLICTS_${PN} = " libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libgbm"