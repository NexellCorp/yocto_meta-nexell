DESCRIPTION = "Nexell bluetooth stack service daemon"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "base"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit linux-nexell-base
ARCH_TYPE_NUM = "${@get_kernel_arch_num(d,"${TARGET_PREFIX}")}"

PV = "1.0"
PR = "r0"

SRC_URI = " \
	file://${ARCH_TYPE_NUM}/bsa \
	"

S = "${WORKDIR}/${ARCH_TYPE_NUM}/bsa"

BSA_SERVICE = "${@bb.utils.contains('DISTRO_FEATURES', 'support-adanis-bt', 'bsa.service-adanis', \
				bb.utils.contains('DISTRO_FEATURES', 'support-azurewave-bt', 'bsa.service-azurewave', 'not_supported', d), d)}"

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

	if [ ${BSA_SERVICE} != "not_supported" ]; then
		install -m 0644 ${S}${systemd_system_unitdir}/${BSA_SERVICE} ${D}${systemd_system_unitdir}/bsa.service
		cd ${D}${systemd_system_unitdir}/multi-user.target.wants
		ln -sf ../bsa.service bsa.service
	else
		echo "Can't find supported BSA service."
	fi
}

FILES_${PN} = "${sysconfdir}/bluetooth ${bindir} ${systemd_system_unitdir} ${systemd_system_unitdir}/multi-user.target.wants"

INSANE_SKIP_${PN} = "ldflags"
