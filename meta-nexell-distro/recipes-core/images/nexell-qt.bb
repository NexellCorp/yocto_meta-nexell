inherit post-process-qt
inherit post-process
require recipes-graphics/images/core-image-weston.bb

DESCRIPTION ?= "Nexell qt images"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

IMAGE_FEATURES += "package-management ssh-server-dropbear splash"

SYSTEMD_INSTALL = " \
	systemd-compat-units \
	rsyslog-systemd \
"

DEVEL_TOOLS = " \
	android-tools-nexell \
"

TOUCH_IMAGE_INSTALL = "${@bb.utils.contains('DISTRO_FEATURES', 'no-use-tslib', '', 'tslib tslib-conf tslib-tests tslib-calibrate tslib-nexell', d)}"

ETC_TOOLS = " \
	tinyalsa \
	udev-extraconf \
	tzdata \
	net-tools \
	openssl \
	curl \
	procps \
"

ETC_LIBS = " \
	${@bb.utils.contains('DISTRO_FEATURES', 'support-ffmpeg', 'ffmpeg', '', d)} \
	id3lib \
	libid3tag \
	libjpeg-turbo \
	icu \
"

NX_QT_LAUNCHER = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.6', 'qtcinema-launcher', \
                     bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.8', 'qtcinema-launcher', \
                     bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', 'qtsmarthome qt5-smarthome-launcher', '', d), d), d)}"

NX_LAUNCHER = "${@bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-eglfs nexell-qt5.6 support-daudio', 'nexell-qtbootlauncher', '${NX_QT_LAUNCHER}', d)}"

ALLGO_CONNECTIVITY = "${@bb.utils.contains('DISTRO_FEATURES', 'support-allgo-connectivity', 'common-api-c++-dbus common-api-c++', '', d)}"

NEXELL_CUSTOMIZE_INSTALL = " \
	${NX_LAUNCHER} \
	${@bb.utils.contains('DISTRO_FEATURES', 'support-daudio', 'nexell-nxlogrotate nexell-bluetooth', '', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-eglfs nexell-qt5.6 support-daudio support-daudio-sdk', 'nexell-daudio-sdk', '', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-eglfs', 'eglfs-kms', '', d)} \
	${ALLGO_CONNECTIVITY} \
	user-fonts \
	nexell-bootanim \
	nexell-nxreboot \
	nexell-qt5-touchsetup \
"
IMAGE_INSTALL_append = " \
	kernel-modules \
	${SYSTEMD_INSTALL} \
	${DEVEL_TOOLS} \
	packagegroup-nexell-qt \
	weston-init \
	rtl-8188eus-${ARCH_TYPE_NUM} \
	testsuite \
	${TOUCH_IMAGE_INSTALL} \
	${ETC_TOOLS} \
	${ETC_LIBS} \
	${NEXELL_CUSTOMIZE_INSTALL} \
"

# pyro -> sumo
# gcc 6.3.0 -> 7.3.0 update: compile error
#${@bb.utils.contains('NEXELL_BOARD_SOCNAME', 's5p4418', 'NxAudioPlayer NxQuickRearCam NxVideoPlayer', '', d)}
