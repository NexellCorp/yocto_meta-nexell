DESCRIPTION = "nx-gst-meta"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=1c29f522bb1f7e5686af40aff4032b50"

SRCREV = "5ec4c79e245b17bbb27991eccf09692ced8023bb"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/nx-gst-meta;protocol=git;branch=yocto-dmc"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"
#COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p4418-avn-ref|s5p6818-avn-ref|s5p4418-navi-ref)"

DEPENDS = "gstreamer1.0 gstreamer1.0-plugins-base glib-2.0"

inherit autotools

#Nexell code : for /usr/include search QA issue: compile-host-path QA issue avoid
EXTRA_OEMAKE += " \
     'libnxgstmeta_la_CFLAGS=$(GST_CFLAGS) -I${STAGING_INCDIR}' \
     "
     
do_configure() {
    cd ${S}
    ./autogen.sh
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

FILES_${PN} = "${libdir} ${includedir}"
FILES_SOLIBSDEV = ""
#ALLOW_EMPTY_${PN} = "1"
