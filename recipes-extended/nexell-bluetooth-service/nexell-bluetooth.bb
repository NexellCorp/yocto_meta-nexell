DESCRIPTION = "Nexell Bluetooth Service"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "base"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit linux-nexell-base
ARCH_TYPE_NUM = "${@get_kernel_arch_num(d,"${TARGET_PREFIX}")}"

PV = "bsa"
PR = "0.1"

SRC_URI = " \
	file://${ARCH_TYPE_NUM}/${PV} \
	"

S = "${WORKDIR}/${ARCH_TYPE_NUM}/${PV}"

do_install() {
	install -d ${D}${sysconfdir}/bluetooth
	install -d ${D}${bindir}
	install -d ${D}${systemd_system_unitdir}/multi-user.target.wants

	install -m 0644 ${S}${sysconfdir}/bluetooth/* ${D}${sysconfdir}/bluetooth/

	if [ "${ARCH_TYPE_NUM}" -eq "32" ]; then
		install -m 0755 ${S}${bindir}/bsa_server ${D}${bindir}/
	else
		echo "Requires 64bit bsa_server."
	fi

	install -m 0644 ${S}${systemd_system_unitdir}/bsa.service ${D}${systemd_system_unitdir}/
	cp -aR ${S}${systemd_system_unitdir}/multi-user.target.wants/bsa.service ${D}${systemd_system_unitdir}/multi-user.target.wants/
}

FILES_${PN} = "${sysconfdir}/bluetooth ${bindir} ${systemd_system_unitdir} ${systemd_system_unitdir}/multi-user.target.wants"

INSANE_SKIP_${PN} = "ldflags"
