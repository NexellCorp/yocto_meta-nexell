inherit linux-nexell-base

DESCRIPTION = "user fonts"
LICENSE = "CLOSED"

TYPE = "fonts"

SRC_URI = "file://font \
           "

S = "${WORKDIR}"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_install () {
    install -d ${D}/usr/share/fonts

    install -m 0755 ${S}/font/*.ttf ${D}/usr/share/fonts/.
}

PACKAGES = "${PN}"
FILES_${PN} += "/usr/share/fonts "
