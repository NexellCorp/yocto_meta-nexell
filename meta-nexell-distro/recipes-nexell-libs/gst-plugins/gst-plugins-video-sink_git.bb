DESCRIPTION = "gst-plugins-video-sink"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=1c29f522bb1f7e5686af40aff4032b50"

SRCREV = "3fdbf2d88e25c9ca463adf4d64db9c41b541124e"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/gst-plugins-video-sink;protocol=git;branch=nexell"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = "nx-gst-meta libdrm-nx"

inherit autotools pkgconfig

EXTRA_OECONF = " \
     '--prefix=${STAGING_DIR_HOST}' \
     "

EXTRA_OEMAKE = " \
     'libgstnxvideosink_la_CFLAGS = $(GST_CFLAGS) -I${STAGING_INCDIR} -I${STAGING_INCDIR}/libdrm' \
     'libgstnxvideosink_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS) -L${STAGING_LIBDIR}' \
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
}

INSANE_SKIP_${PN} = "compile-host-path dev-so debug-files"
FILES_${PN} = "${libdir}"
ALLOW_EMPTY_${PN} = "1"
