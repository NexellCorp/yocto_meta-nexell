SUMMARY = "Qt5 touch setup script"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
    file://nexell-qt5-touchsetup.sh \
"

S = "${WORKDIR}"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/nexell-qt5-touchsetup.sh ${D}${bindir}/
}

FILES_${PN} = " ${bindir}"
