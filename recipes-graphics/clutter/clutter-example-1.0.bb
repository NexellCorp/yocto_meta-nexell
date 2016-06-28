# Copyright (C) 2016 Sungwoo Park <swpark@nexell.co.kr>
# Released under the MIT license (see COPYING.MIT for the terms)

DESCRIPTION = "Clutter example app"
HOMEPAGE = "http://www.clutter-project.org"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING;md5=42e3c987b7344c398e5e62d39b989cd4"

PN = "clutter-example"
PR = "r0"

DEPENDS = "clutter-1.25"

FILEXEXTRAPATHS_prepend := "${THISDIR}/clutter-example-1.0:"
SRC_URI = "file://clutter-example.tar.bz2"

S = "${WORKDIR}/clutter-example"

COMPATIBLE_MACHINE = "artik710-raptor"
PACKAGE_ARCH = "${MACHINE_ARCH}"

FILES_${PN} += " \
    ${bindir}/actor-model \
    ${bindir}/basic-actor \
    ${bindir}/bin-layout \
    ${bindir}/box-layout \
    ${bindir}/canvas \
    ${bindir}/constraints \
    ${bindir}/drag-action \
    ${bindir}/easing-modes \
    ${bindir}/flow-layout \
    ${bindir}/grid-layout \
    ${bindir}/image-content \
    ${bindir}/layout-manager \
    ${bindir}/pan-action \
    ${bindir}/rounded-rectangle \
    ${bindir}/scroll-actor \
    ${bindir}/threads"


MY_CFLAGS = "-I${STAGING_INCDIR} -I${STAGING_INCDIR}/glib-2.0 -I${STAGING_LIBDIR}/glib-2.0/include -I${STAGING_INCDIR}/gdk-pixbuf-2.0 -I${STAGING_INCDIR}/clutter-1.0 -I${STAGING_INCDIR}/cairo -I${STAGING_INCDIR}/cogl -I${STAGING_INCDIR}/pango-1.0 -I${STAGING_INCDIR}/atk-1.0 -I${STAGING_INCDIR}/json-glib-1.0"
MY_LDFLAGS = "-L${STAGING_LIBDIR}"
MY_LIBS = "-lclutter-1.0 -lglib-2.0 -lgobject-2.0 -lgio-2.0 -lcogl-gles2 -lcogl-pango -lcogl-path -lcogl -lcairo -lgdk_pixbuf-2.0 -lm"

do_compile() {
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o actor-model ${WORKDIR}/clutter-example/actor-model.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o basic-actor ${WORKDIR}/clutter-example/basic-actor.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o bin-layout ${WORKDIR}/clutter-example/bin-layout.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o box-layout ${WORKDIR}/clutter-example/box-layout.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o canvas ${WORKDIR}/clutter-example/canvas.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o constraints ${WORKDIR}/clutter-example/constraints.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o drag-action ${WORKDIR}/clutter-example/drag-action.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o easing-modes ${WORKDIR}/clutter-example/easing-modes.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o flow-layout ${WORKDIR}/clutter-example/flow-layout.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o grid-layout ${WORKDIR}/clutter-example/grid-layout.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o image-content ${WORKDIR}/clutter-example/image-content.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o layout-manager ${WORKDIR}/clutter-example/layout-manager.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o pan-action ${WORKDIR}/clutter-example/pan-action.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o rounded-rectangle ${WORKDIR}/clutter-example/rounded-rectangle.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o scroll-actor ${WORKDIR}/clutter-example/scroll-actor.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o threads ${WORKDIR}/clutter-example/threads.c
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 actor-model ${D}${bindir}
    install -m 0755 basic-actor ${D}${bindir}
    install -m 0755 bin-layout ${D}${bindir}
    install -m 0755 box-layout ${D}${bindir}
    install -m 0755 canvas ${D}${bindir}
    install -m 0755 constraints ${D}${bindir}
    install -m 0755 drag-action ${D}${bindir}
    install -m 0755 easing-modes ${D}${bindir}
    install -m 0755 flow-layout ${D}${bindir}
    install -m 0755 grid-layout ${D}${bindir}
    install -m 0755 image-content ${D}${bindir}
    install -m 0755 layout-manager ${D}${bindir}
    install -m 0755 pan-action ${D}${bindir}
    install -m 0755 rounded-rectangle ${D}${bindir}
    install -m 0755 scroll-actor ${D}${bindir}
    install -m 0755 threads ${D}${bindir}
}

PROVIDES = "clutter-example"
RPROVIDES_${PN} = "clutter-example"
