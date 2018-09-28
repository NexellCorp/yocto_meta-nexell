inherit linux-nexell-base

DESCRIPTION = "sdl test"
LICENSE = "CLOSED"
SECTION = "libs"

TYPE = "mali-nexell-sdl-test"

SRC_URI = "file://sdl_imageview_test \
           "
  
S = "${WORKDIR}"

DEPENDS = "libsdl2"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
    cd ${S}/sdl_imageview_test

    oe_runmake CROSSNAME=${TARGET_PREFIX} \
               INCLUDES="-I./ -I${STAGING_INCDIR} -I${STAGING_INCDIR}/SDL2" \
               LDFLAGS="-L${STAGING_LIBDIR} -L${STAGING_BASELIBDIR}"
}

do_install () {
    install -d ${D}${bindir}/sdl_example/img

    install -m 0755 ${S}/sdl_imageview_test/main ${D}${bindir}/sdl_example/sdl_imageview_test
    install -m 0755 ${S}/sdl_imageview_test/img/* ${D}${bindir}/sdl_example/img/
}

INSANE_SKIP_${PN} = "ldflags"
FILES_${PN} = "${bindir}/sdl_example"
