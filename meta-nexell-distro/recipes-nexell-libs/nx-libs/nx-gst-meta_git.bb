DESCRIPTION = "nx-gst-meta"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=1c29f522bb1f7e5686af40aff4032b50"

SRCREV = "23a97a7a53b1a817f4dc8bd22b0dfc639f649ab1"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/nx-gst-meta;protocol=git;branch=nexell"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = "gstreamer1.0 gstreamer1.0-plugins-base glib-2.0"

inherit autotools pkgconfig

#Nexell code : for /usr/include search QA issue: compile-host-path QA issue avoid
EXTRA_OEMAKE += " \
     'libnxgstmeta_la_CFLAGS=$(GST_CFLAGS) -I${STAGING_INCDIR}' \
"

FILES_${PN} = "${libdir} ${includedir}"
FILES_SOLIBSDEV = ""
