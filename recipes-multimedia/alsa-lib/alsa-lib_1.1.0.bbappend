FILESEXTRAPATHS_prepend := "${THISDIR}/patch:"

SRC_URI_append = " \
	file://nexell-alsa-lib-support-pcm-link-spdif.patch \
	"
