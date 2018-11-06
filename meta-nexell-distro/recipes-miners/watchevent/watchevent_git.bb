SUMMARY = "Nexell watchevent"
LICENSE = "CLOSED"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

inherit pkgconfig systemd

SYSTEMD_SERVICE_${PN} = "watchevent.service"

SRC_URI = "https://gitlab.com/opentemp/watchevent.git;protocol=git;branch=bitminer \
"
SRC_URI_append = " \
		   file://watchevent.service \
"

SRCREV = "${AUTOREV}"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
	oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
	oe_runmake CROSS_COMPILE=${TARGET_PREFIX} INCLUDES="-I${STAGING_INCDIR} -I${STAGING_INCDIR}/nexell" LDFLAGS="-L${STAGING_LIBDIR}" CC="$CC"
#	oe_runmake
}

do_install_append() {
	install -d ${D}${sbindir}
	install -m 0755 ${S}/watchevent ${D}${sbindir}
	install -d ${D}${systemd_unitdir}/system
	install -m 0644 ${WORKDIR}/watchevent.service ${D}${systemd_unitdir}/system
	sed -i -e 's,@SBINDIR@,${sbindir},g' \
		-e 's,@BINDIR@,${bindir},g' \
		-e 's,@SYSCONFDIR@,${sysconfdir},g' \
		-e 's,@BASE_BINDIR@,${base_bindir},g' \
		${D}${systemd_unitdir}/system/watchevent.service
}

#PACKAGES = "${PN}"
#FILES_${PN} = "${bindir}"
INSANE_SKIP_${PN} = "ldflags"
INSANE_SKIP += "build-deps"
ALLOW_EMPTY_${PN} = "1"

