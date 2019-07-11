inherit linux-nexell-base

DESCRIPTION = "Nexell init process"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "base"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

PV = "1.0"
PR = "r0"

SRCREV = "${AUTOREV}"

SRC_URI = "git://review.gerrithub.io/NexellCorp/nx_init;protocol=https;branch=master"

S = "${WORKDIR}/git"

inherit autotools pkgconfig gettext

do_compile() {
	cd ${S}
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC"
}

do_install() {
	install -d ${D}${base_sbindir}
	install -m 0755 ${S}/nx_init ${D}${base_sbindir}
}

FILES_${PN} = "${base_sbindir}"

INSANE_SKIP_${PN} = "already-stripped"
