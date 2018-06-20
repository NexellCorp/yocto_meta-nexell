FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append += " \
	file://wlan0_nl80211_interfaces \
	file://init.wlan0 \
	file://network.conf.factory \
"

FILES_${PN} += " config"

do_install_append() {
	install -m 0755 ${WORKDIR}/wlan0_nl80211_interfaces ${D}${sysconfdir}/network/interfaces
	install -m 0755 ${WORKDIR}/init.wlan0 ${D}${sysconfdir}/init.d/networking
	install -d ${D}/config
	install -m 0400 ${WORKDIR}/network.conf.factory ${D}/config
}

