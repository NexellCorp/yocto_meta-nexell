FILESEXTRAPATHS_prepend := "${THISDIR}/patch:"

SRC_URI_append = " \
	file://capi-dbus-allgo-support.patch \
	"
