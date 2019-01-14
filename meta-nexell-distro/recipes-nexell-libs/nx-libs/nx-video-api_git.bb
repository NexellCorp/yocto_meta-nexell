DESCRIPTION = "nx-video-api"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=1c29f522bb1f7e5686af40aff4032b50"

SRCREV = "012a0a25ca79de075f68a24105c1cf1a3c5127c4"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/nx-video-api;protocol=git;branch=nexell"

S = "${WORKDIR}/git"

PV = "1"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = "libdrm-nx"

inherit autotools

#Nexell code : for /usr/include search QA issue: compile-host-path QA issue avoid
EXTRA_OECONF = " \
    '--prefix=${STAGING_DIR_HOST}' \
    "

EXTRA_OEMAKE += " \
    'AM_CFLAGS=$(WARN_CFLAGS) -I./include -I${STAGING_INCDIR}' \
    'libnx_video_api_la_LDFLAGS = -L${STAGING_LIBDIR}' \
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
    install -d ${D}${libdir}
    install -d ${D}${includedir}

    cd ${S}
    oe_runmake install DESTDIR=${D}
    cp -apr ${D}/* ${BASE_WORKDIR}/extra-rootfs-support/
}

INSANE_SKIP_${PN} = "dev-so invalid-packageconfig"
FILES_${PN} = "${libdir} ${includedir}"
FILES_${PN}-dev = "${includedir}"
FILES_SOLIBSDEV = ""
#ALLOW_EMPTY_${PN} = "1"
