# Copyright (C) 2016 Sungwoo Park <swpark@nexell.co.kr>
# Released under the MIT license (see COPYING.MIT for the terms)

DESCRIPTION = "MX library tests"
HOMEPAGE = "http://www.clutter-project.org"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING;md5=a999c60d99a036044fe26ef139635c03"

PN = "mx-tests"
PR = "r0"

DEPENDS = "mx-1.0"

FILEXEXTRAPATHS_prepend := "${THISDIR}/mx-tests-1.0:"
SRC_URI = "file://mx-tests.tar.bz2"

S = "${WORKDIR}/mx-tests"

COMPATIBLE_MACHINE = "(s5p4418-avn-ref|s5p4418-navi-ref|s5p6818-avn-ref|s5p6818-artik710-raptor)"
PACKAGE_ARCH = "${MACHINE_ARCH}"

FILES_${PN} += " \
    ${bindir}/test-box-layout \
    ${bindir}/test-containers \
    ${bindir}/test-deform-texture-interactive \
    ${bindir}/test-deform-texture \
    ${bindir}/test-draggable \
    ${bindir}/test-droppable \
    ${bindir}/test-focus \
    ${bindir}/test-grid \
    ${bindir}/test-label \
    ${bindir}/test-mx \
    ${bindir}/test-path-bar \
    ${bindir}/test-script \
    ${bindir}/test-shaders \
    ${bindir}/test-stack \
    ${bindir}/test-table-2 \
    ${bindir}/test-table \
    ${bindir}/test-texture-frame \
    ${bindir}/test-views \
    ${bindir}/test-widgets \
    ${bindir}/test-window \
"

MY_CFLAGS = "-I${STAGING_INCDIR} -I${STAGING_INCDIR}/glib-2.0 -I${STAGING_LIBDIR}/glib-2.0/include -I${STAGING_INCDIR}/gdk-pixbuf-2.0 -I${STAGING_INCDIR}/clutter-1.0 -I${STAGING_INCDIR}/cairo -I${STAGING_INCDIR}/cogl -I${STAGING_INCDIR}/pango-1.0 -I${STAGING_INCDIR}/atk-1.0 -I${STAGING_INCDIR}/json-glib-1.0 -I${STAGING_INCDIR}/mx-1.0"
MY_LDFLAGS = "-L${STAGING_LIBDIR}"
MY_LIBS = "-lmx-1.0 -lclutter-1.0 -lglib-2.0 -lgobject-2.0 -lgio-2.0 -lcogl-gles2 -lcogl-pango -lcogl-path -lcogl -lcairo -lgdk_pixbuf-2.0 -lm"

do_compile() {
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-box-layout ${WORKDIR}/mx-tests/test-box-layout.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-containers ${WORKDIR}/mx-tests/test-containers.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-deform-texture-interactive ${WORKDIR}/mx-tests/test-deform-texture-interactive.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-deform-texture ${WORKDIR}/mx-tests/test-deform-texture.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -lpango-1.0  -o test-draggable ${WORKDIR}/mx-tests/test-draggable.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-droppable ${WORKDIR}/mx-tests/test-droppable.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-focus ${WORKDIR}/mx-tests/test-focus.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-grid ${WORKDIR}/mx-tests/test-grid.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-mx ${WORKDIR}/mx-tests/test-mx.c ${WORKDIR}/mx-tests/test-button-group.c ${WORKDIR}/mx-tests/test-label.c ${WORKDIR}/mx-tests/test-entry.c ${WORKDIR}/mx-tests/test-buttons.c ${WORKDIR}/mx-tests/test-combo-box.c ${WORKDIR}/mx-tests/test-progress-bar.c ${WORKDIR}/mx-tests/test-slider.c ${WORKDIR}/mx-tests/test-toggle.c ${WORKDIR}/mx-tests/test-tooltips.c ${WORKDIR}/mx-tests/test-expander.c ${WORKDIR}/mx-tests/test-scroll-grid.c ${WORKDIR}/mx-tests/test-scroll-bar.c ${WORKDIR}/mx-tests/test-scroll-view.c ${WORKDIR}/mx-tests/test-styles.c ${WORKDIR}/mx-tests/test-dialog.c ${WORKDIR}/mx-tests/test-spinner.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-path-bar ${WORKDIR}/mx-tests/test-path-bar.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-script ${WORKDIR}/mx-tests/test-script.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-shaders ${WORKDIR}/mx-tests/test-shaders.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-stack ${WORKDIR}/mx-tests/test-stack.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-table-2 ${WORKDIR}/mx-tests/test-table-2.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-table ${WORKDIR}/mx-tests/test-table.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-texture-frame ${WORKDIR}/mx-tests/test-texture-frame.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-views ${WORKDIR}/mx-tests/test-views.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-widgets ${WORKDIR}/mx-tests/test-widgets.c
    ${CC} ${MY_CFLAGS} ${MY_LDFLAGS} ${MY_LIBS} -o test-window ${WORKDIR}/mx-tests/test-window.c
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 test-box-layout ${D}${bindir}
    install -m 0755 test-containers ${D}${bindir}
    install -m 0755 test-deform-texture-interactive ${D}${bindir}
    install -m 0755 test-deform-texture ${D}${bindir}
    install -m 0755 test-draggable ${D}${bindir}
    install -m 0755 test-droppable ${D}${bindir}
    install -m 0755 test-focus ${D}${bindir}
    install -m 0755 test-grid ${D}${bindir}
    install -m 0755 test-mx ${D}${bindir}
    install -m 0755 test-path-bar ${D}${bindir}
    install -m 0755 test-script ${D}${bindir}
    install -m 0755 test-shaders ${D}${bindir}
    install -m 0755 test-stack ${D}${bindir}
    install -m 0755 test-table-2 ${D}${bindir}
    install -m 0755 test-table ${D}${bindir}
    install -m 0755 test-texture-frame ${D}${bindir}
    install -m 0755 test-views ${D}${bindir}
    install -m 0755 test-widgets ${D}${bindir}
    install -m 0755 test-window ${D}${bindir}
}

PROVIDES = "mx-tests"
RPROVIDES_${PN} = "mx-tests"
