DESCRIPTION = "Nexell init process"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "base"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

PV = "1.0"
PR = "r0"

SRCREV = "${AUTOREV}"

SRC_URI = " \
	file://nx_init \
"

S = "${WORKDIR}/nx_init"

do_install() {
	install -d ${D}${base_sbindir}

	install -m 0755 ${S}/nx_init ${D}${base_sbindir}

	if [ -f ${S}/NxQuickRearCam_not_strip ]; then
		install -m 0755 ${S}/NxQuickRearCam_not_strip ${D}${base_sbindir}/NxQuickRearCam
	fi
}

FILES_${PN} = "${base_sbindir}"
