SUMMARY = "Qt5 demo cinema auto running"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
    file://qt_cinema_run.sh \
    file://qt_cinema.service \
    file://eglfs_config-navi_ref.json \
    file://eglfs_config-conv_svmc.json
"

S = "${WORKDIR}"

inherit systemd

SYSTEMD_SERVICE_${PN} = "qt_cinema.service"

EGLFS_CONF = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-navi-ref', 'eglfs_config-navi_ref.json', \
                bb.utils.contains('DISTRO_FEATURES', 'nexell-convergence-svmc', 'eglfs_config-conv_svmc.json', 'not_supported', d), d)}"

do_install() {
    install -d ${D}${bindir}
    install -d ${D}${systemd_system_unitdir}
    install -d ${D}${sysconfdir}/qboot

    install -m 0755 ${S}/qt_cinema_run.sh ${D}${bindir}/
    install -m 0644 ${S}/qt_cinema.service ${D}${systemd_system_unitdir}/

    if [ ${EGLFS_CONF} != "not_supported" ]; then
        install -m 0755 ${S}/${EGLFS_CONF} ${D}${sysconfdir}/qboot/eglfs_config.json
    else
        echo "Can't find supported EGLFS configuration."
    fi
}

FILES_${PN} += "${bindir} ${systemd_system_unitdir} ${sysconfdir}/qboot"

INSANE_SKIP_${PN} = "host-user-contaminated"
