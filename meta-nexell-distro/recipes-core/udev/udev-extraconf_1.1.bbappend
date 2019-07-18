FILESEXTRAPATHS_prepend := "${THISDIR}/patch:"

SRC_URI_append = " \
	file://support-utf8-udev-automount-vfat.patch \
	${@bb.utils.contains('DISTRO_FEATURES', 'support-daudio-sdk', 'file://nexell-automount-disable.patch', '', d)} \
"
