inherit linux-nexell-base

SUMMARY = "Nexell qt bootlauncher"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
	file://launcher-capacitive.conf \
	file://launcher-resistive.conf \
	file://eglfs_config.json \
	file://nx_platform_env.sh \
	file://Makefile \
	file://nx_qtbootlauncher.c \
"

S = "${WORKDIR}"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
	if [ "${NEXELL_TOUCH_CLASS}" != "CAPACITIVE" ]; then
		oe_runmake CROSS_COMPILE="${TARGET_PREFIX}" CC="${CC}" CFLAGS="${CFLAGS} -DTSLIB"
	else
		oe_runmake CROSS_COMPILE="${TARGET_PREFIX}" CC="${CC}" CFLAGS="${CFLAGS}"
	fi
}

do_install() {
	install -d ${D}${systemd_unitdir}/system-generators
	install -d ${D}${sysconfdir}/profile.d
	install -d ${D}${sysconfdir}/qboot

	install -m 0755 ${S}/nx_qtbootlauncher ${D}${systemd_unitdir}/system-generators/
	install -m 0644 ${S}/nx_platform_env.sh ${D}${sysconfdir}/profile.d/

	if [ "${NEXELL_TOUCH_CLASS}" = "CAPACITIVE" ]; then
		install -m 0755 ${S}/launcher-capacitive.conf ${D}${sysconfdir}/qboot/launcher.conf
	else
		install -m 0755 ${S}/launcher-resistive.conf ${D}${sysconfdir}/qboot/launcher.conf
	fi

	install -m 0755 ${S}/eglfs_config.json ${D}${sysconfdir}/qboot/
}

FILES_${PN} = "${systemd_unitdir}/system-generators ${sysconfdir}/profile.d ${sysconfdir}/qboot"
INSANE_SKIP_${PN} = "installed-vs-shipped already-stripped"
