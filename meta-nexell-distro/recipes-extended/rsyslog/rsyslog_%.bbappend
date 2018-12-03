FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += "	\
			file://rsyslog.conf	\
			file://rsyslog.logrotate	\
			file://rotate_cgminerlog.sh	\
			"


do_install_append() {
	install -d ${D}${sysconfdir}/logrotate.d
	install -m 644 ${WORKDIR}/rsyslog.logrotate ${D}${sysconfdir}/logrotate.d/rsyslog
	install -m 755 ${WORKDIR}///rotate_cgminerlog.sh ${D}${sysconfdir}/
}

