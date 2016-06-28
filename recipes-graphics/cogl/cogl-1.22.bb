SUMMARY = "Modern 3D graphics API with associated utility APIs"
HOMEPAGE = "http://wiki.clutter-project.org/wiki/Cogl"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING;md5=1b1a508d91d25ca607c83f92f3e31c84"
SRC_URI = "http://ftp.acc.umu.se/pub/GNOME/sources/cogl/1.22/cogl-1.22.0.tar.xz"
SRC_URI[sha256sum] = "689dfb5d14fc1106e9d2ded0f7930dcf7265d0bc84fa846b4f03941633eeaa91"
SRC_URI[md5sum] = "ab684ec96848d79d22757fb3064820c8"

#FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-${PV}:"

S = "${WORKDIR}/${PN}.0"

inherit autotools pkgconfig gtk-doc gettext

COMPATIBLE_MACHINE = "artik710-raptor"
PACKAGE_ARCH = "${MACHINE_ARCH}"

#DEPENDS = "glib-2.0 gdk-pixbuf cairo pango"
DEPENDS = "glib-2.0 gdk-pixbuf cairo pango virtual/libgles2"
RDEPENDS_${PN} += "virtual/egl virtual/libgles2"

PACKAGES =+ "${PN}-examples \
             libcogl libcogl-gles2 libcogl-pango libcogl-path \
            "
AUTOTOOLS_AUXDIR = "${S}/build"

# TODO : .bbappend, dynamic configuration

EXTRA_OECONF += "--disable-introspection \
             --enable-examples-install \
             --enable-debug \
             --disable-gles1 \
             --enable-cairo \
             --disable-gl \
             --disable-glx \
             --enable-gles2 \
             --enable-cogl-gles2 \
             --enable-null-egl-platform \
             --disable-kms-egl-platform \
             --disable-xlib-egl-platform \
             --disable-wayland-egl-platform \
             --disable-wayland-egl-server \
             --enable-cogl-pango \"

do_install_append() {
    cp ${S}/cogl/cogl-egl.h ${D}${includedir}/cogl/cogl
    cp ${S}/../build/cogl/cogl-egl-defines.h ${D}${includedir}/cogl/cogl
}

FILES_${PN}-examples = "${bindir}/* ${datadir}/cogl/examples-data/*"
FILES_libcogl = "${libdir}/libcogl${SOLIBS}"
FILES_libcogl-gles2 = "${libdir}/libcogl-gles2${SOLIBS}"
FILES_libcogl-pango = "${libdir}/libcogl-pango${SOLIBS}"
FILES_libcogl-path = "${libdir}/libcogl-path${SOLIBS}"

RDEPENDS_libcogl-pango = "virtual/egl"

PROVIDES = "cogl-1.22"
RPROVIDES_libcogl = "cogl-1.22"
RCONFLICTS_libcogl = "cogl-1.22"
RREPLACES_libcogl = "cogl-1.22"
