FILESEXTRAPATHS_prepend := "${THISDIR}/patch:"

SRC_URI_append = " \
	file://support-utf8-udev-automount-vfat.patch \
	"
