DESCRIPTION = "nx-drm-allocator"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=df9b0758c6dcd94963adedfa6f9f0580"

SRCREV = "{AUTOREV}"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/nx-drm-allocator;protocol=git;branch=nexell"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit autotools

#Nexell code : for /usr/include/libdrm search QA issue: compile-host-path QA issue avoid
EXTRA_OEMAKE += " \
    'AM_CFLAGS=$(WARN_CFLAGS) -I./ -I./drm' \
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
#ALLOW_EMPTY_${PN} = "1"
