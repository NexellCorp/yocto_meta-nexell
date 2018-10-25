DESCRIPTION = "nx-drm-allocator"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=df9b0758c6dcd94963adedfa6f9f0580"

SRCREV = "afac0284ef123e2d330a671558f1be46783f2c20"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/nx-drm-allocator;protocol=git;branch=nexell"

S = "${WORKDIR}/git"

PV = "1"
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
