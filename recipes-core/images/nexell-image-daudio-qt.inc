DESCRIPTION = "Extend core-image by adding Qt5 for DAudio"
LICENSE = "GPLv2"

inherit core-image populate_sdk_qt5
require recipes-graphics/images/core-image-weston.bb

WAYLAND_IMAGE_INSTALL = "${@base_contains('DISTRO_FEATURES', 'wayland', \
	'qtwayland \
	 qtwayland-plugins', '', d)}"

QT_IMAGE_INSTALL_addon = " \
	qtquickcontrols \
	qtquickcontrols2 \
	qtmultimedia \
	qtmultimedia-plugins \
	qtmultimedia-qmlplugins \
	${WAYLAND_IMAGE_INSTALL} \
	"
TOUCH_IMAGE_INSTALL = " \
	tslib \
	tslib-conf \
	tslib-calibrate \
	tslib-nexell \
	"

QT_ALSA_IMAGE_INSTALL = " \
	alsa-lib \
	"

QT_ALSA_UTILS_IMAGE_INSTALL = " \
	alsa-utils \
	alsa-utils-alsamixer \
	alsa-utils-aplay \
	alsa-utils-amixer \
	alsa-utils-speakertest \
	alsa-utils-alsactl \
	"

NEXELL_CUSTOMIZE_INSTALL = " \
	user-fonts \
	qtbootlauncher \
	"

IMAGE_INSTALL += " \
	${QT_IMAGE_INSTALL_addon} \
	${TOUCH_IMAGE_INSTALL} \
	${QT_ALSA_IMAGE_INSTALL} \
	${QT_ALSA_UTILS_IMAGE_INSTALL} \
	${NEXELL_CUSTOMIZE_INSTALL} \
	"
