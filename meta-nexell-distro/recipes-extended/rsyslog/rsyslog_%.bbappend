FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += "	\
			file://rsyslog.conf	\
			file://rsyslog.logrotate	\
			"


do_install_append() {
	install -m 644 ${WORKDIR}/rsyslog.logrotate ${D}${sysconfdir}/logrotate.d/rsyslog
}

