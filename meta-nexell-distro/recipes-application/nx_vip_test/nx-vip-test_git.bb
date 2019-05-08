DESCRIPTION = "nx_vip_test"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=1c29f522bb1f7e5686af40aff4032b50"

SRC_URI = " \
	file://nx_vip_test \
	"
S = "${WORKDIR}/nx_vip_test"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = "nx-v4l2 nx-video-api nx-drm-allocator"

inherit autotools pkgconfig

EXTRA_OECONF = " \
	'--prefix=${STAGING_DIR_HOST}' \
	"

do_configure() {
	cd ${S}
	NOCONFIGURE=true ./autogen.sh
	oe_runconf ${EXTRA_OECONF}
}

do_compile() {
	cd ${S}
	oe_runmake clean
	oe_runmake \
		AM_CPPFLAGS+="$(WARN_CFLAGS) \
			-I. \
			-I${STAGING_INCDIR} \
			-I${STAGING_INCDIR}/libdrm \
			-I${S}/src/include \
			"
}

do_install() {
	cd ${S}
	install -d ${D}${bindir}
	oe_runmake install DESTDIR=${D}
}

INSANE_SKIP_${PN} = "installed-vs-shipped"
FILES_${PN} = "${bindir}"
ALLOW_EMPTY_${PN} = "1"
