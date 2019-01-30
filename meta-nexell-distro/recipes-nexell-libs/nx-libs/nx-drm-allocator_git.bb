DESCRIPTION = "nx-drm-allocator"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=df9b0758c6dcd94963adedfa6f9f0580"

PV ?= "1.0+git${SRCPV}"
SRCREV = "${AUTOREV}"

SRC_URI = "git://review.gerrithub.io/NexellCorp/linux_library_nx-drm-allocator;protocol=https;branch=nexell"

S = "${WORKDIR}/git"

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
