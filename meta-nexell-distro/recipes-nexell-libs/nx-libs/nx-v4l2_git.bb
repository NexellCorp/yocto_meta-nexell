DESCRIPTION = "nx-v4l2"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=50caa96ef586a321a3228191e14a18ea"

PV ?= "1.0+git${SRCPV}"
SRCREV = "${AUTOREV}"

SRC_URI = "git://review.gerrithub.io/NexellCorp/linux_library_nx-v4l2;protocol=https;branch=nexell"

S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit autotools

do_configure() {
    cd ${S}
    NOCONFIGURE=true ./autogen.sh
    oe_runconf --enable-static
}

do_compile_prepend() {
    #overwrite v4l2-controls.h to latest linux_libc_header
    cp ${BASE_WORKDIR}/clone_kernel_src/include/uapi/linux/v4l2-controls.h ${STAGING_INCDIR}/linux/
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

#-------------------------------------------------------------------------------------
#libnx_v4l2.so.0.0.0 location ==> ./tmp/sysroots/avn-4418/usr/lib/libnx_v4l2.so.0.0.0
#------------------------------------------------------------------------------------
