FILESEXTRAPATHS_prepend := "${THISDIR}/patch:"

SRC_URI_append = " \
	file://capi-dbus-add-send-with-reply-set-notify.patch \
	file://capi-dbus-add-support-for-custom-marshalling.patch \
	file://capi-dbus-block-acquire-io-path-on-send.patch \
	"
