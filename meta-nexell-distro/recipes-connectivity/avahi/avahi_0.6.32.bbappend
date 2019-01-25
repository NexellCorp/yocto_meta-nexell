FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-1.0:"

SRC_URI += "file://miner.service"

SYSTEMD_PATH = "${systemd_unitdir}/system/"
AUTOSTART_SYSTEMD_PATH = "/etc/systemd/system/multi-user.target.wants/"

FILES_${PN} += "${AUTOSTART_SYSTEMD_PATH}avahi-daemon.service"

do_install_append(){
	install -d ${D}${AUTOSTART_SYSTEMD_PATH}
	ln -sf ${SYSTEMD_PATH}avahi-daemon.service ${D}${AUTOSTART_SYSTEMD_PATH}avahi-daemon.service
	rm -f ${D}${sysconfdir}/avahi/services/sftp-ssh.service
	rm -f ${D}${sysconfdir}/avahi/services/ssh.service
	install -m 0644 ${WORKDIR}/miner.service ${D}${sysconfdir}/avahi/services/
}
