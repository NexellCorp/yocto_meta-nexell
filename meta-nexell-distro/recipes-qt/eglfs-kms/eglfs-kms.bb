SUMMARY = "Qt5 qpa eglfs kms configuration"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
	file://eglfs_config-daudio_ref.json \
	file://eglfs_config-navi_ref.json \
	file://eglfs_config-conv_daudio.json \
	file://eglfs_config-conv_svmc.json \
"

S = "${WORKDIR}"

PV = "NEXELL"
PR = "0.1"

EGLFS_CONF = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-daudio-ref', 'eglfs_config-daudio_ref.json', \
				bb.utils.contains('DISTRO_FEATURES', 'nexell-navi-ref', 'eglfs_config-navi_ref.json', \
				bb.utils.contains('DISTRO_FEATURES', 'nexell-convergence-daudio', 'eglfs_config-conv_daudio.json', \
				bb.utils.contains('DISTRO_FEATURES', 'nexell-convergence-svmc', 'eglfs_config-conv_svmc.json', 'not_supported', d), d), d), d)}"

do_install() {
	install -d ${D}${sysconfdir}/qboot

	if [ ${EGLFS_CONF} != "not_supported" ]; then
		install -m 0755 ${S}/${EGLFS_CONF} ${D}${sysconfdir}/qboot/eglfs_config.json
	else
		echo "Can't find supported qt5 qpa eglfs kms configuration."
	fi
}

FILES_${PN} = "${sysconfdir}/qboot"
