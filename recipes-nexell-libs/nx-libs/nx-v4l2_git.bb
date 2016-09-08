DESCRIPTION = "nx-v4l2"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=50caa96ef586a321a3228191e14a18ea"

SRCREV = "07b95ba3cf5abdc7441a0934c893533832095435"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/nx-v4l2;protocol=git;branch=yocto-dmc"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit autotools

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

#-------------------------------------------------------------------------------------
#libnx_v4l2.so.0.0.0 location ==> ./tmp/sysroots/avn-4418/usr/lib/libnx_v4l2.so.0.0.0
#------------------------------------------------------------------------------------
