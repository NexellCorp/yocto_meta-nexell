DESCRIPTION = "nx-renderer"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=16672d7f6b1e95223937ee9eaf04e0c4"

SRCREV = "bc365673700815a7e8339d17bcd7b3892c3ea595"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/nx-renderer;protocol=git;branch=nexell"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = "libdrm-nx gstreamer1.0 gstreamer1.0-plugins-base glib-2.0"

inherit autotools

#Nexell code : for /usr/include search QA issue: compile-host-path QA issue avoid
EXTRA_OECONF = " \
    '--prefix=${STAGING_DIR_HOST}' \
    '--with-extrapath=${STAGING_EXECPREFIXDIR}' \
    '--with-extrapath_lib=${STAGING_LIBDIR}' \
    '--with-extrapath_include=${STAGING_INCDIR}' \
    "

EXTRA_OEMAKE += " \
    'AM_CFLAGS=$(WARN_CFLAGS) -I./include -I${STAGING_INCDIR} -I${STAGING_INCDIR}/libdrm' \
    'libnx_renderer_la_LDFLAGS = -L${STAGING_LIBDIR}' \
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
    oe_runmake install DESTDIR=${D}
}

INSANE_SKIP_${PN} = "dev-so"
FILES_${PN} = "${libdir} ${includedir}"
FILES_SOLIBSDEV = ""
RDEPENDS_${PN} += "libdrm"
#ALLOW_EMPTY_${PN} = "1"