SUMMARY = "Qt5 demo smarthome auto running"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "qtsmarthome"

SRC_URI = " \
    file://qt5_smarthome_run_eglfs.sh \
    file://qt5_smarthome_run_xcb.sh \
    file://qt5-smarthome.service \
"

S = "${WORKDIR}"

BACKEND = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', bb.utils.contains('DISTRO_FEATURES', 'x11', 'xcb', 'eglfs', d), 'eglfs', d)}"
inherit systemd

SYSTEMD_SERVICE_${PN} = "qt5-smarthome.service"

do_install() {
    install -d ${D}${bindir}
    install -d ${D}${systemd_unitdir}/system
    #install -d ${D}${systemd_unitdir}/system/multi-user.target.wants/

    install -m 0755 ${S}/qt5_smarthome_run_${BACKEND}.sh ${D}${bindir}/qt5_smarthome_run.sh
    install -m 0644 ${S}/qt5-smarthome.service ${D}${systemd_unitdir}/system
}

FILES_${PN} += " ${bindir}"

INSANE_SKIP_${PN} = "host-user-contaminated"