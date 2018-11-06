FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

PACKAGECONFIG_append = " networkd resolved timesyncd"
#USERADD_PARAM_${PN} += "--system -d / -M --shell /bin/nologin systemd-network;"
#USERADD_PARAM_${PN} += "--system -d / -M --shell /bin/nologin systemd-resolve;"
#USERADD_PARAM_${PN} += "--system -d / -M --shell /bin/nologin systemd-timesync;"

SRC_URI_append = "file://0001-reboot-param-warning-error-bug-fixed.patch"

SRC_URI_append = " \
    file://local.rules \
    file://usb1-rules.sh \
    file://usb2-rules.sh \
    file://10-eth.network \
    file://30-wlan.network \
    file://60-usb.network \
    file://resolv.conf \
	file://timesyncd.conf \
"

IMAGE_INSTALL += "10-eth.network.factory"
FILES_${PN} += "${sysconfdir}/10-eth.network.factory"

do_install_append() {
    install -d ${D}${sysconfdir}/udev/rules.d/
    install -m 0644 ${WORKDIR}/local.rules ${D}${sysconfdir}/udev/rules.d/

    install -d ${D}${sysconfdir}/udev/scripts/ 
    install -m 0755 ${WORKDIR}/usb1-rules.sh ${D}${sysconfdir}/udev/scripts/
    install -m 0755 ${WORKDIR}/usb2-rules.sh ${D}${sysconfdir}/udev/scripts/

    install -d ${D}${sysconfdir}/systemd/network/
    install -m 0644 ${WORKDIR}/10-eth.network ${D}${sysconfdir}/systemd/network/
    install -m 0644 ${WORKDIR}/10-eth.network ${D}${sysconfdir}/10-eth.network.factory
    install -m 0644 ${WORKDIR}/30-wlan.network ${D}${sysconfdir}/systemd/network/
    install -m 0644 ${WORKDIR}/60-usb.network ${D}${sysconfdir}/systemd/network/

    # Allow automount from udev
    install -m 0644 ${D}${systemd_system_unitdir}/systemd-udevd.service ${D}${sysconfdir}/systemd/system/
    sed -i 's/MountFlags=slave/MountFlags=shared/g' ${D}${sysconfdir}/systemd/system/systemd-udevd.service

	install -m 0644 ${WORKDIR}/resolv.conf ${D}${sysconfdir}/resolv.conf

	# enable timesyncd service
	install -m 0644 ${WORKDIR}/timesyncd.conf ${D}${sysconfdir}/systemd
	install -d ${D}${sysconfdir}/systemd/system/sysinit.target.wants
	sed -i -e 's,PrivateTmp=yes,PrivateTmp=no,g' \
		${D}${systemd_unitdir}/system/systemd-timesyncd.service
	ln -sf ${systemd_unitdir}/system/systemd-timesyncd.service \
		${D}${sysconfdir}/systemd/system/sysinit.target.wants/systemd-timesyncd.service

	sed -i -e 's,PrivateTmp=yes,PrivateTmp=no,g' \
		${D}${systemd_unitdir}/system/systemd-resolved.service
}
