inherit linux-nexell-base

DESCRIPTION = "User fonts"
LICENSE = "CLOSED"

TYPE = "fonts"

SRC_URI = " \
	file://fonts \
	"

S = "${WORKDIR}"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_install () {
    install -d ${D}${datadir}/fonts/ttf

    install -m 0755 ${S}/fonts/*.ttf ${D}${datadir}/fonts/ttf/
}

PACKAGES = "${PN}"
FILES_${PN} += "${datadir}/fonts/ttf"
