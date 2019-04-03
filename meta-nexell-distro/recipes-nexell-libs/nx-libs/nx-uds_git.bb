DESCRIPTION = "nexell unix domain socket wrapper"

LICENSE = "CLOSED"

PV ?= "1.0+git${SRCPV}"
SRCREV = "${AUTOREV}"

SRC_URI = "git://github.com/NexellCorp/linux_library_nx-uds.git;protocol=https;branch=master"

S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit autotools

# Nexell code : for /usr/include search QA issue: compile-host-path QA issue avoid
EXTRA_OECONF = " \
	'--prefix=${STAGING_DIR_HOST}' \
"

EXTRA_OEMAKE += " \
	'AM_CFLAGS=$(WARN_CFLAGS) -I./include -I${STAGING_INCDIR}' \
	'libnx_uds_la_LDFLAGS = -L${STAGING_LIBDIR}' \
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
