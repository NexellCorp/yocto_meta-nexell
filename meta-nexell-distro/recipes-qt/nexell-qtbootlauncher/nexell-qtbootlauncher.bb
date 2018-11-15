inherit linux-nexell-base

SUMMARY = "Nexell qt bootlauncher"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
	file://launcher-capacitive.conf \
	file://launcher-resistive.conf \
	file://eglfs_config-daudio_ref.json \
	file://eglfs_config-conv_daudio.json \
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

EGLFS_CONF = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-daudio-ref', 'eglfs_config-daudio_ref.json', \
				bb.utils.contains('DISTRO_FEATURES', 'nexell-convergence-daudio', 'eglfs_config-conv_daudio.json', 'not_supported', d), d)}"

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

	if [ ${EGLFS_CONF} != "not_supported" ]; then
		install -m 0755 ${S}/${EGLFS_CONF} ${D}${sysconfdir}/qboot/eglfs_config.json
	else
		echo "Can't find supported EGLFS configuration."
	fi
}

FILES_${PN} = "${systemd_unitdir}/system-generators ${sysconfdir}/profile.d ${sysconfdir}/qboot"
INSANE_SKIP_${PN} = "installed-vs-shipped already-stripped"
