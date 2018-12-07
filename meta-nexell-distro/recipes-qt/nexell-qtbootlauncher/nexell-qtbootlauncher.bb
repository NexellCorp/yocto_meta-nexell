SUMMARY = "Nexell Qt bootlauncher"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
	file://launcher-capacitive.conf \
	file://launcher-resistive.conf \
	file://Makefile \
	file://nx_qtbootlauncher.c \
"

S = "${WORKDIR}"

PV = "NEXELL"
PR = "0.1"

do_compile() {
	if [ "${NEXELL_TOUCH_CLASS}" != "CAPACITIVE" ]; then
		oe_runmake CROSS_COMPILE="${TARGET_PREFIX}" CC="${CC}" CFLAGS="${CFLAGS} -DTSLIB"
	else
		oe_runmake CROSS_COMPILE="${TARGET_PREFIX}" CC="${CC}" CFLAGS="${CFLAGS}"
	fi
}

do_install() {
	install -d ${D}${systemd_unitdir}/system-generators
	install -d ${D}${sysconfdir}/qboot

	install -m 0755 ${S}/nx_qtbootlauncher ${D}${systemd_unitdir}/system-generators/

	if [ "${NEXELL_TOUCH_CLASS}" = "CAPACITIVE" ]; then
		install -m 0755 ${S}/launcher-capacitive.conf ${D}${sysconfdir}/qboot/launcher.conf
	else
		install -m 0755 ${S}/launcher-resistive.conf ${D}${sysconfdir}/qboot/launcher.conf
	fi
}

FILES_${PN} = "${systemd_unitdir}/system-generators ${sysconfdir}/qboot"
