DESCRIPTION = "gst-plugins-video-enc"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=1c29f522bb1f7e5686af40aff4032b50"

PV ?= "1.0+git${SRCPV}"
SRCREV = "${AUTOREV}"

SRC_URI = "git://review.gerrithub.io/NexellCorp/linux_library_gst-plugins-video-enc;protocol=https;branch=nexell"

S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = "nx-gst-meta nx-video-api libdrm"

inherit autotools pkgconfig

EXTRA_OECONF = " \
     '--prefix=${STAGING_DIR_HOST}' \
     "

EXTRA_OEMAKE = " \
     'libgstnxvideoenc_la_CFLAGS = $(GST_CFLAGS) -I${STAGING_INCDIR}' \
     'libgstnxvideoenc_la_CFLAGS += -I${STAGING_INCDIR}/libdrm' \
     'libgstnxvideoenc_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS) -L${STAGING_LIBDIR}' \
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
