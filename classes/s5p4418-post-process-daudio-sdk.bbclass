DESCRIPTION = "Nexell DAudio Partial Post-update"
HOMEPAGE = "http://www.nexell.co.kr"
LICENSE = "CLOSED"

postprocess_install_daudio_sdk_function() {
	install -m 0644 ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files${systemd_system_unitdir}/android-tools-adbd.service ${IMAGE_ROOTFS}${systemd_system_unitdir}/
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_install_daudio_sdk_function;"
