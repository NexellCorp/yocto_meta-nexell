inherit linux-nexell-base

SUMMARY = "Nexell qt bootlauncher"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
	file://launcher.conf \
	file://eglfs_config.json \
	file://nx_platform_env.sh \
	file://Makefile \
	file://nx_qtbootlauncher.c \
"

S = "${WORKDIR}"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_install() {
	install -d ${D}${systemd_unitdir}/system-generators
	install -d ${D}${sysconfdir}/profile.d
	install -d ${D}${sysconfdir}/qboot

	install -m 0755 ${S}/nx_qtbootlauncher ${D}${systemd_unitdir}/system-generators/
	install -m 0644 ${S}/nx_platform_env.sh ${D}${sysconfdir}/profile.d/
	install -m 0755 ${S}/launcher.conf ${D}${sysconfdir}/qboot/
	install -m 0755 ${S}/eglfs_config.json ${D}${sysconfdir}/qboot/
}

FILES_${PN} = "${systemd_unitdir}/system-generators ${sysconfdir}/profile.d ${sysconfdir}/qboot"
INSANE_SKIP_${PN} = "installed-vs-shipped already-stripped"
