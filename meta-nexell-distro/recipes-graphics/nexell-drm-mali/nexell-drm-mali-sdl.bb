inherit linux-nexell-base

DESCRIPTION = "Nexell Mali library"
LICENSE = "CLOSED"
SECTION = "libs"

TYPE = "mali-nexell"

SRC_URI = "file://library \
           file://sdl/src \
           "

S = "${WORKDIR}"

#PROVIDES += "virtual/egl virtual/libgles1 virtual/libgles2 virtual/libopencl virtual/libwayland-egl"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
    cd ${S}/sdl/src/ES1_tutorial1
    oe_runmake CROSSNAME=${TARGET_PREFIX} INCLUDE="-I${S}/sdl/src/3rdparty/include/khronos -I${STAGING_INCDIR} -I${S}/sdl/src/ES1_tutorial1"

    cd ${S}/sdl/src/ES2_tutorial1
    oe_runmake CROSSNAME=${TARGET_PREFIX} INCLUDE="-I${S}/sdl/src/3rdparty/include/khronos -I${STAGING_INCDIR} -I${S}/sdl/src/ES2_tutorial1"
}

do_install () {
    install -d ${D}${bindir}/sdl_example
    install -d ${D}${libdir}
    install -d ${D}${libdir}/nexell

    install -m 0644 ${S}/library/${ARCH_TYPE_NUM}/*.so ${D}${libdir}/nexell/
    install -m 0755 ${S}/sdl/src/ES1_tutorial1/tutorial1_es1 ${D}${bindir}/sdl_example/
    install -m 0755 ${S}/sdl/src/ES2_tutorial1/tutorial1_es2 ${D}${bindir}/sdl_example/
    install -m 0644 ${S}/sdl/src/ES2_tutorial1/t1_rend.frag  ${D}${bindir}/sdl_example/
    install -m 0644 ${S}/sdl/src/ES2_tutorial1/t1_rend.vert  ${D}${bindir}/sdl_example/


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
}

INSANE_SKIP_${PN} = "already-stripped debug-files dev-so ldflags"
PACKAGES = "${PN}"
FILES_${PN} += "${bindir}/sdl_example ${libdir}"

RREPLACES_${PN} = " libegl libglesv1-cm1 libglesv2-2"
RPROVIDES_${PN} = " libegl libglesv1-cm1 libglesv2-2"
RPROVIDES_${PN} += " libGLESv2.so libEGL.so libGLESv1_CM.so"
RCONFLICTS_${PN} = " libegl libglesv1-cm1 libglesv2-2 "
