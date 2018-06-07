DESCRIPTION = "Nexell Firmware Update Service"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "console/utils"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

PV = "nx_update"
PR = "0.1"
SRCREV = "${AUTOREV}"

SRC_URI = " \
	file://${PV} \
	"

S = "${WORKDIR}/${PV}"

CFLAGS_remove_arm = "-g"

do_install() {
	install -d ${D}${sysconfdir}
	install -d ${D}${bindir}
	install -d ${D}${systemd_system_unitdir}/multi-user.target.wants

	install -m 0755 ${S}${sysconfdir}/* ${D}${sysconfdir}/
	install -m 0644 ${S}${systemd_system_unitdir}/nxupdate.service ${D}${systemd_system_unitdir}/
	cp -aR ${S}${systemd_system_unitdir}/multi-user.target.wants/nxupdate.service ${D}${systemd_system_unitdir}/multi-user.target.wants/

	install -m 0755 ${S}/nx_update ${D}${bindir}/
}

FILES_${PN} = "${sysconfdir} ${bindir} ${systemd_system_unitdir} ${systemd_system_unitdir}/multi-user.target.wants"
