DESCRIPTION = "Nexell log management service daemon"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "console/utils"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

PV = "1.0"
PR = "r0"

SRCREV = "${AUTOREV}"

SRC_URI = " \
	file://nxlogrotate \
	"

S = "${WORKDIR}/nxlogrotate"

CFLAGS_remove_arm = "-g"
CXXFLAGS_remove_arm = "-g"

do_install() {
	install -d ${D}${systemd_system_unitdir}/multi-user.target.wants
	install -d ${D}${sbindir}

	install -m 0644 ${S}${systemd_system_unitdir}/nxlogrotate.service ${D}${systemd_system_unitdir}/
	cp -aR ${S}${systemd_system_unitdir}/multi-user.target.wants/nxlogrotate.service ${D}${systemd_system_unitdir}/multi-user.target.wants/

	install -m 0755 ${S}/nxlogrotate ${D}${sbindir}/
}

FILES_${PN} = "${sbindir} ${systemd_system_unitdir} ${systemd_system_unitdir}/multi-user.target.wants"
