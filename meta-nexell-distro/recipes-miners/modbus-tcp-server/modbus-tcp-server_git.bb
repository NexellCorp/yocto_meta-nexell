SUMMARY = "Nexell modbus tcp server for miner"
LICENSE = "CLOSED"

DEPENDS = "libmodbus"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

inherit pkgconfig systemd

SYSTEMD_SERVICE_${PN} = "modbus-tcp-server.service"

SRC_URI = "https://gitlab.com/opentemp/modbus-tcp-server.git;protocol=git;branch=bitminer \
"
SRC_URI_append = " \
		   file://modbus-tcp-server.service \
"

SRCREV = "${AUTOREV}"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile_prepend() {
	oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
}

do_compile() {
	oe_runmake CROSS_COMPILE=${TARGET_PREFIX} INCLUDES="-I${STAGING_INCDIR} -I${STAGING_INCDIR}/nexell -I${S}" LDFLAGS="-L${STAGING_LIBDIR}" CC="$CC"
#	oe_runmake
}

do_install_append() {
	install -d ${D}${bindir}
	install -m 0755 ${S}/modbus-tcp-server ${D}${bindir}
	install -d ${D}${systemd_unitdir}/system
	install -m 0644 ${WORKDIR}/modbus-tcp-server.service ${D}${systemd_unitdir}/system
	sed -i -e 's,@SBINDIR@,${sbindir},g' \
		-e 's,@BINDIR@,${bindir},g' \
		-e 's,@SYSCONFDIR@,${sysconfdir},g' \
		-e 's,@BASE_BINDIR@,${base_bindir},g' \
		${D}${systemd_unitdir}/system/modbus-tcp-server.service
}

#PACKAGES = "${PN}"
#FILES_${PN} = "${bindir}"
INSANE_SKIP_${PN} = "ldflags"
INSANE_SKIP += "build-deps"
ALLOW_EMPTY_${PN} = "1"

