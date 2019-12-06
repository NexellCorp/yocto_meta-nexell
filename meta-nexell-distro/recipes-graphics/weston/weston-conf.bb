SUMMARY = "Weston configuration files"
HOMEPAGE = "http://wayland.freedesktop.org"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

PACKAGE_ARCH = "${MACHINE_ARCH}"

SRC_URI = "file://wayland_env.sh \
           file://weston.ini \
           file://weston-recalibrate.sh \
           file://weston_run.sh \
           file://weston.service \
           file://weston.conf \
"
inherit systemd
SYSTEMD_SERVICE_${PN} = "weston.service"

do_install () {
    install -d ${D}${sysconfdir}/profile.d
    install -d ${D}${bindir}
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/weston.ini ${D}${sysconfdir}
    install -m 0644 ${WORKDIR}/weston.conf ${D}${sysconfdir}
    install -m 0644 ${WORKDIR}/wayland_env.sh ${D}${sysconfdir}/profile.d/
    install -m 0755 ${WORKDIR}/weston-recalibrate.sh ${D}${bindir}
	install -m 0755 ${WORKDIR}/weston_run.sh ${D}${bindir}/
	install -m 0644 ${WORKDIR}/weston.service ${D}${systemd_system_unitdir}/
}

FILES_${PN} += "${sysconfdir}/profile.d/* ${sysconfdir}/weston.ini ${bindir}/weston-recalibrate.sh ${bindir} ${systemd_system_unitdir}"

INSANE_SKIP_${PN} = "host-user-contaminated"
