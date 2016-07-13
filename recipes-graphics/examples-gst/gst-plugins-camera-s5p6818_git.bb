DESCRIPTION = "gst-plugins-camera-s5p6818"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=85c196593bd0c183d13480e405a91969"

SRCREV = "f192cd08c637e7d109f716afef7c2e12667b8d7f"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/gst-plugins-camera-s5p6818;protocol=git;branch=artik"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"
COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p4418-avn-ref)"

DEPENDS = "nx-v4l2 nx-drm-allocator nx-gst-meta gstreamer1.0 gstreamer1.0-plugins-base glib-2.0"

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
    'libgstnxcamerasrc_la_LDFLAGS = -L${STAGING_LIBDIR} -L${STAGING_LIBDIR}/${GST_PLUGIN_PATH} -L${EXTRAPATH_LIB}' \
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
ALLOW_EMPTY_${PN} = "1"
