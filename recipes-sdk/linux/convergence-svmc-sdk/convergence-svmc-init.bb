DESCRIPTION = "convergence-svmc service - for nexell convergence-svmc board"
SECTION = "app"
LICENSE = "CLOSED"

SRC_URI = " file://convergence-svmc.service \
          "
S = "${WORKDIR}/convergence-svmc-init"

inherit systemd

SYSTEMD_SERVICE_${PN} = "convergence-svmc.service"

do_install() {
    install -d ${D}${systemd_unitdir}/system

    install -m 0644 ${WORKDIR}/convergence-svmc.service ${D}${systemd_unitdir}/system
}

FILES_${PN} = "${bindir} "
