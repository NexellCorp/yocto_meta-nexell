FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += "	\
			file://rsyslog.conf	\
			file://rsyslog.logrotate	\
			"

