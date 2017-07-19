DESCRIPTION = "gst-plugins-renderer"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=17ed5b4c1158fe2f827528f761c39970"

SRCREV = "1e81fd567ed9b6394aae56a7068f975ca3b5ba6f"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/gst-plugins-renderer;protocol=git;branch=nexell"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = "nx-v4l2 nx-drm-allocator nx-renderer gstreamer1.0 gstreamer1.0-plugins-base glib-2.0 libdrm-nx"

inherit autotools pkgconfig

EXTRA_OECONF = " \
    '--prefix=${STAGING_DIR_HOST}' \
    '--with-extrapath=${STAGING_EXECPREFIXDIR}' \
    '--with-extrapath_lib=${STAGING_LIBDIR}' \
    '--with-extrapath_include=${STAGING_INCDIR}' \
"

EXTRA_OEMAKE = " \
    'AM_CFLAGS=$(WARN_CFLAGS) \
               -I${STAGING_INCDIR} \
               -I${STAGING_INCDIR}/libdrm \
               -I./ \
               -I${EXTRAPATH_INCLUDE} \
               -I${EXTRAPATH}/lib/glib-2.0/include \
               -I${EXTRAPATH_INCLUDE}/glib-2.0 \
               -I${EXTRAPATH_INCLUDE}/gstreamer-${GST_API_VERSION} \
               -I${STAGING_LIBDIR}/gstreamer-${GST_API_VERSION}/include' \
    'libgstnxrenderer_la_LDFLAGS = -L${STAGING_LIBDIR} -L${EXTRAPATH_LIB}' \
"

do_configure() {
    cd ${S}
    NOCONFIGURE=true ./autogen.sh
    oe_runconf
}

do_compile() {
    cd ${S}
    oe_runmake clean
    oe_runmake
}

do_install() {
    cd ${S}
    install -d ${D}${libdir}/gstreamer-1.0
    oe_runmake install DESTDIR=${D}
}

INSANE_SKIP_${PN} = "compile-host-path dev-so debug-files"
FILES_${PN} = "${libdir} ${includedir}"
RDEPENDS_${PN} += "libdrm"
ALLOW_EMPTY_${PN} = "1"