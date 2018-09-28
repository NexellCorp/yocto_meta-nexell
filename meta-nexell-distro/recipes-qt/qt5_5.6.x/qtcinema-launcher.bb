SUMMARY = "Qt5 demo cinema auto running"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
    file://qt_cinema_run.sh \
    file://qt_cinema.service \
"

S = "${WORKDIR}"

inherit systemd

SYSTEMD_SERVICE_${PN} = "qt_cinema.service"

do_install() {
    install -d ${D}${bindir}
    install -d ${D}${systemd_unitdir}/system

    install -m 0755 ${S}/qt_cinema_run.sh ${D}${bindir}/
    install -m 0644 ${S}/qt_cinema.service ${D}${systemd_unitdir}/system/
}

FILES_${PN} += " ${bindir}"

INSANE_SKIP_${PN} = "host-user-contaminated"