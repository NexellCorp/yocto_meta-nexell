SUMMARY = "SDL2 OpenGL ES Test Applications"
HOMEPAGE = "https://github.com/thp/sdl2-opengles-test"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://main_glesv2.cpp;beginline=1;endline=26;md5=0d08cb5b4a92c955950cc46dbaa3b6f0"

DEPENDS = "libsdl2 libsdl2-image libsdl2-mixer"

inherit pkgconfig

TARGETS = "sdl2_opengles1_test sdl2_opengles2_test sdl2_image_test sdl2_mixer_test"

PV = "1.0.6+gitr${SRCPV}"
SRC_URI = "git://github.com/thp/sdl2-opengles-test \
           file://opengles_test \
           file://opengles_test/0001-DATADIR-remove.patch \
"

S = "${WORKDIR}/git"

SRCREV = "40041a4e3425aab6d2a7170148a3dc36975f2d09"

do_compile() {
    oe_runmake TARGETS="${TARGETS}" CXXFLAGS="${CXXFLAGS} ${LDFLAGS} -I${STAGING_INCDIR}/SDL2" DATADIR="${S}/images"
}

do_install() {
    for f in ${TARGETS}; do
        install -d ${D}${bindir}/sdl_examples/$f
        install -m 0755 ${S}/$f ${D}${bindir}/sdl_examples/$f/
        install -m 0644 ${WORKDIR}/opengles_test/$f-appinfo.json ${D}${bindir}/sdl_examples/$f/appinfo.json
    done

    install -d ${D}${bindir}/sdl_examples/sdl2_image_test/images
    install -d ${D}${bindir}/sdl_examples/sdl2_mixer_test/sounds
    #install -d ${D}${bindir}/sdl_examples/sdl2_ttf_test/fonts

    install -m 0644 ${S}/images/* ${D}${bindir}/sdl_examples/sdl2_image_test/images/
    install -m 0644 ${S}/sounds/* ${D}${bindir}/sdl_examples/sdl2_mixer_test/sounds/
    #install -m 0644 ${S}/fonts/* ${D}${bindir}/sdl_examples/sdl2_ttf_test/fonts/
}

FILES_${PN} += "${bindir}"
FILES_${PN}-dbg += "${bindir}/*/.debug"
