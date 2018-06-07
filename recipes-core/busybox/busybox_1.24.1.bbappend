FILESEXTRAPATHS_prepend := "${THISDIR}/patch:"

SRC_URI_append = " \
	file://nexell-blkid-label-uuid-exception.patch \
	"
