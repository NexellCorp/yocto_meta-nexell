SUMMARY = "Graphics library for creating hardware-accelerated user interfaces"
HOMEPAGE = "http://www.clutter-project.org/"
LICENSE = "LGPLv2.1+"
LIC_FILES_CHKSUM = "file://COPYING;md5=4fbd65380cdd255951079008b364516c"
SRC_URI = "http://ftp.acc.umu.se/pub/gnome/sources/clutter/1.25/clutter-1.25.2.tar.xz"
SRC_URI[sha256sum] = "55f8816cd3556831a259fd437e3a23774a29e914791575d4bc98fbc03fa02ee9"
SRC_URI[md5sum] = "8f1349bdb214d97b922b3b834ab0208c"

S = "${WORKDIR}/${PN}.2"

inherit autotools pkgconfig gtk-doc gettext

DEPENDS = "pango glib-2.0 json-glib atk udev cogl-1.22 libxkbcommon xkeyboard-config"
PACKAGE_BEFORE_PN += "${PN}-examples"
AUTOTOOLS_AUXDIR = "${S}/build"

EXTRA_OECONF += "--disable-introspection \
             --disable-quartz-backend \
             --disable-win32-backend \
             --disable-gdk-backend \
             --disable-cex100-backend \
             --disable-tslib-input \
             --disable-x11-backend \
             --enable-evdev-input \
             --enable-examples \
             --disable-wayland-backend \
             --disable-wayland-compositor"

# do_install_append() {
#     # install examples
#     install -d ${D}${libdir}/clutter/examples
#     cp ${WORKDIR}/build/examples/actor-model ${D}${libdir}/clutter/examples
#     cp ${WORKDIR}/build/examples/basic-actor ${D}${libdir}/clutter/examples
#     cp ${WORKDIR}/build/examples/box-layout ${D}${libdir}/clutter/examples
#     cp ${WORKDIR}/build/examples/canvas ${D}${libdir}/clutter/examples
#     cp ${WORKDIR}/build/examples/constraints ${D}${libdir}/clutter/examples
#     cp ${WORKDIR}/build/examples/drag-action ${D}${libdir}/clutter/examples
#     cp ${WORKDIR}/build/examples/easing-modes ${D}${libdir}/clutter/examples
# }

#FILES_${PN} += "${libdir}/clutter/examples"
FILES_${PN}-dbg += "${libdir}/clutter/examples/.debug"
FILES_${PN}-examples = "${libdir}/clutter/examples"

FILES_${PN}-dbg += "${libexecdir}/installed-tests/.debug"

PROVIDES = "clutter-1.25"
