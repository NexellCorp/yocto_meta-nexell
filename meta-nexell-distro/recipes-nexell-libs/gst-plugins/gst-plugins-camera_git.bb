DESCRIPTION = "gst-plugins-camera"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=ce23c541b32b443603093d2be5f59a24"

SRCREV = "d005024198dea6f3b4df9654696860119cfabb8d"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/gst-plugins-camera;protocol=git;branch=nexell"

S = "${WORKDIR}/git"

PV = "1"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = "nx-v4l2 nx-drm-allocator nx-gst-meta gstreamer1.0 gstreamer1.0-plugins-base glib-2.0"

inherit autotools pkgconfig

EXTRA_OECONF = " \
    '--prefix=${PKG_CONFIG_SYSROOT_DIR}' \
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
    install -d ${D}${libdir}
    oe_runmake install DESTDIR=${D}
    cp -apr ${D}/* ${BASE_WORKDIR}/extra-rootfs-support/
}

INSANE_SKIP_${PN} = "compile-host-path dev-so debug-files invalid-packageconfig"
FILES_${PN} = "${libdir}"
ALLOW_EMPTY_${PN} = "1"
PROVIDES = "gst-plugins-camera"
RPROVIDES_${PN} = "gst-plugins-camera"
