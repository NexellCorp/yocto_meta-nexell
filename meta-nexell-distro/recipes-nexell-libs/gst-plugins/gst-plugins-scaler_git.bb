DESCRIPTION = "gst-plugins-scaler"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=d4e4d3ffd18ad8cf7d8b31e70366a8ca"

SRCREV = "168b91a000c3c27e6d4a6820de7fb3310b17415f"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/gst-plugins-scaler;protocol=git;branch=nexell"

S = "${WORKDIR}/git"

PV = "1"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = "nx-scaler nx-drm-allocator nx-gst-meta gstreamer1.0 gstreamer1.0-plugins-base glib-2.0"

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
               -I./ \
               -I${EXTRAPATH_INCLUDE} \
               -I${EXTRAPATH}/lib/glib-2.0/include \
               -I${EXTRAPATH_INCLUDE}/glib-2.0 \
               -I${EXTRAPATH_INCLUDE}/gstreamer-${GST_API_VERSION} \ 
               -I${STAGING_LIBDIR}/gstreamer-${GST_API_VERSION}/include' \
    'libgstnxscaler_la_LDFLAGS = -L${STAGING_LIBDIR} -L${EXTRAPATH_LIB}' \
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
    cp -apr ${D}/* ${BASE_WORKDIR}/extra-rootfs-support/
}

INSANE_SKIP_${PN} = "compile-host-path dev-so debug-files invalid-packageconfig"
FILES_${PN} = "${libdir} ${includedir}"
ALLOW_EMPTY_${PN} = "1"
