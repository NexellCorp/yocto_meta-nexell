DESCRIPTION = "cluster service - for nexell cluster board"
SECTION = "app"
LICENSE = "CLOSED"

SRC_URI = " file://cluster.service \
          "
S = "${WORKDIR}/clusrer-init"

inherit systemd

SYSTEMD_SERVICE_${PN} = "cluster.service"

do_install() {
    install -d ${D}${systemd_unitdir}/system

    install -m 0644 ${WORKDIR}/cluster.service ${D}${systemd_unitdir}/system
}

FILES_${PN} = "${bindir} "
