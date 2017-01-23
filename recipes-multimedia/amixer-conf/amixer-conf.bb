DESCRIPTION = "amixer setup - for nexell board"
SECTION = "console/utils"
LICENSE = "CLOSED"

DEPENDS = "alsa-utils"

SRC_URI = " file://amixer-conf.sh \
            file://amixer-conf.service \
          "
S = "${WORKDIR}/amixer-conf"

inherit systemd

SYSTEMD_SERVICE_${PN} = "amixer-conf.service"

do_install() {
    install -d ${D}${bindir}
    install -d ${D}${systemd_unitdir}/system

    install -m 0644 ${WORKDIR}/amixer-conf.service ${D}${systemd_unitdir}/system
    install -m 0755 ${WORKDIR}/amixer-conf.sh ${D}${bindir}
}

FILES_${PN} = "${bindir} "