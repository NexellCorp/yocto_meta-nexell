SUMMARY = "Nexell modbus tcp server for miner"
LICENSE = "CLOSED"

DEPENDS = "libmodbus"

SRC_URI = "https://gitlab.com/opentemp/modbus-tcp-server.git;protocol=git;branch=bitminer \
	"
#SRC_URI = "file://* \
#	"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

inherit pkgconfig

SRCREV = "${AUTOREV}"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
	oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
	oe_runmake CROSS_COMPILE=${TARGET_PREFIX} INCLUDES="-I${STAGING_INCDIR} -I${STAGING_INCDIR}/nexell" LDFLAGS="-L${STAGING_LIBDIR}" CC="$CC"
#	oe_runmake
}

do_install_append() {
	install -d ${D}${bindir}
	install -m 0755 ${S}/modbus-tcp-server ${D}${bindir}
}

#PACKAGES = "${PN}"
#FILES_${PN} = "${bindir}"
INSANE_SKIP_${PN} = "ldflags"
#INSANE_SKIP += "build-deps"
ALLOW_EMPTY_${PN} = "1"

