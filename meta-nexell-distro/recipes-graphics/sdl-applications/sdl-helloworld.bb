inherit linux-nexell-base

DESCRIPTION = "sdl test"
LICENSE = "CLOSED"
SECTION = "libs"

TYPE = "mali-nexell-sdl-test"

SRC_URI = "file://helloworld \
           file://nexell-sdl-app-launch.service \
           "
  
S = "${WORKDIR}"

DEPENDS = "libsdl2"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit systemd

SYSTEMD_SERVICE_${PN} = "nexell-sdl-app-launch.service"

do_compile() {
    cd ${S}/helloworld

    oe_runmake CROSSNAME=${TARGET_PREFIX} \
               INCLUDES="-I./ -I${STAGING_INCDIR} -I${STAGING_INCDIR}/SDL2" \
               LDFLAGS="-L${STAGING_LIBDIR} -L${STAGING_BASELIBDIR}"
}

do_install () {
    install -d ${D}${bindir}/sdl_example
    install -d ${D}${systemd_unitdir}/system

    install -m 0755 ${S}/helloworld/sdl_helloWorld ${D}${bindir}/sdl_example/

    install -m 0644 ${S}/nexell-sdl-app-launch.service ${D}${systemd_unitdir}/system/
}

INSANE_SKIP_${PN} = "ldflags"
FILES_${PN} = "${bindir}/sdl_example ${systemd_unitdir}/system"
