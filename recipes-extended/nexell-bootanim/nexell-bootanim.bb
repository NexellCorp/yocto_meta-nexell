DESCRIPTION = "Nexell Bootanimation"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "base"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "libpng"

PV = "bootanimation"
PR = "0.1"
SRCREV = "${AUTOREV}"

SRC_URI = " \
	file://${PV} \
	"

S = "${WORKDIR}/${PV}"

CFLAGS_remove_arm = "-O2 -g"
CFLAGS_append = " -O3 -fPIC"
CFLAGS_prepend = "-I${STAGING_INCDIR}"
LDFLAGS_append = " -static"
LDFLAGS_prepend = "-L${STAGING_LIBDIR} "

do_install() {
	install -d ${D}${bindir}
	install -d ${D}${sysconfdir}/qboot
	install -d ${D}/system/media

	install -m 0755 ${S}${sysconfdir}/qboot/animation.conf ${D}${sysconfdir}/qboot/
	install -m 0755 ${S}/system/media/bootanimation.zip ${D}/system/media/
	install -m 0755 ${S}/bootanimation ${D}${bindir}/
}

FILES_${PN} = "${bindir} ${sysconfdir}/qboot system/media"
