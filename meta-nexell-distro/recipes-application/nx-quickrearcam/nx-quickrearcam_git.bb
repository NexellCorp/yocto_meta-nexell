inherit linux-nexell-base

DESCRIPTION = "Nexell NxQuickRearCam process"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "base"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

PV = "1.0"
PR = "r0"

SRCREV = "${AUTOREV}"

SRC_URI = "git://review.gerrithub.io/NexellCorp/nx_quickrearcam;protocol=https;branch=master"

S = "${WORKDIR}/git"

inherit autotools pkgconfig gettext

do_compile() {
	:
}

do_install() {
	install -d ${D}${base_sbindir}
	if [ -f ${S}/NxQuickRearCam ]; then
		install -m 0755 ${S}/NxQuickRearCam ${D}${base_sbindir}/NxQuickRearCam
	fi
}

FILES_${PN} = "${base_sbindir}"

INSANE_SKIP_${PN} = "already-stripped"
