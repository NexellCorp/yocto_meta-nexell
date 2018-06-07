DESCRIPTION = "Nexell Reboot"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "base"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

PV = "nx_reboot"
PR = "0.1"
SRCREV = "${AUTOREV}"

SRC_URI = " \
	file://${PV} \
	"

S = "${WORKDIR}/${PV}"

CFLAGS_remove_arm = "-g"
CXXFLAGS_remove_arm = "-g"

do_install() {
	install -d ${D}${sbindir}

	install -m 0755 ${S}/nx_reboot ${D}${sbindir}/
}

FILES_${PN} = "${sbindir}"
