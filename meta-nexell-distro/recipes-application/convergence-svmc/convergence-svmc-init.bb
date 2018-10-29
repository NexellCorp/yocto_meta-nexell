DESCRIPTION = "convergence-svmc service - for nexell convergence-svmc board"
SECTION = "app"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

PV = "1.0"
PR = "r0"

SRC_URI = " file://convergence-svmc \
          "
S = "${WORKDIR}/convergence-svmc"

inherit systemd

SYSTEMD_SERVICE_${PN} = "convergence-svmc.service"

do_install() {

    install -d ${D}${systemd_unitdir}/system

    install -m 0644 ${S}/convergence-svmc.service ${D}${systemd_unitdir}/system
}

FILES_${PN} = "${bindir}"

