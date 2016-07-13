SUMMARY = "Qt5 app for checking display colors"
HOMEPAGE = "http://www.jumpnowtek.com"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += "qtbase"

PR = "r1"

SRCREV = "${AUTOREV}"
SRC_URI = "git://github.com/scottellis/qcolorcheck.git"

S = "${WORKDIR}/git"

require recipes-qt/qt5/qt5.inc
COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p4418-avn-ref|s5p4418-navi-ref)"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${B}/qcolorcheck ${D}${bindir}
}

FILES_${PN} = "${bindir}"

RDEPENDS_${PN} = "qtbase-plugins"
