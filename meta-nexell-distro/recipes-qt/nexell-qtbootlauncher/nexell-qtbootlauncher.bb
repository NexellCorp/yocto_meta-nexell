# Nexell Qt bootlauncher

SUMMARY = "Nexell Qt bootlauncher"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
	file://launcher-eglfs-no_tslib.conf \
	file://launcher-eglfs-tslib-no_invertx.conf \
	file://launcher-eglfs-tslib-invertx.conf \
	file://launcher-linuxfb-no_tslib.conf \
	file://launcher-linuxfb-tslib-no_invertx.conf \
	file://launcher-linuxfb-tslib-invertx.conf \
	file://Makefile \
	file://nx_qtbootlauncher.c \
"

S = "${WORKDIR}"
PV = "NEXELL"
PR = "0.1"

QT_BOOTLAUNCHER_PLATFORM = \
	"${@bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-eglfs', 'eglfs', \
	    bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-linuxfb', 'linuxfb', \
	    bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-directfb', 'directfb', 'unknown', \
	    d), d), d)}"
QT_BOOTLAUNCHER_TSLIB = \
	"${@bb.utils.contains('DISTRO_FEATURES', 'no-use-tslib', 'no_tslib', 'tslib', d)}"
QT_BOOTLAUNCHER_TSLIB_INV = \
	"${@bb.utils.contains('DISTRO_FEATURES', 'no-invertx-tslib', 'no_invertx', 'invertx', d)}"

TSDEVICE ?= ""
EVDEV_TOUCHSCREEN_PARAMETERS ?= ""

BOOTTIME_LAUNCHER ?= "0"

do_compile() {
	platform="${QT_BOOTLAUNCHER_PLATFORM}"
	[ ${platform} = "directfb" ] && platform="linuxfb";

	oe_runmake CROSS_COMPILE="${TARGET_PREFIX}" CC="${CC}" CFLAGS="${CFLAGS} \
		-D$(echo $platform | tr a-z A-Z) \
		-D$(echo ${QT_BOOTLAUNCHER_TSLIB} | tr a-z A-Z) \
		-DBOOTTIME_LAUNCHER=${BOOTTIME_LAUNCHER}"
}

do_install() {
	output="${D}${sysconfdir}/qboot/launcher.conf"
	platform="${QT_BOOTLAUNCHER_PLATFORM}"
	[ ${platform} = "directfb" ] && platform="linuxfb";

	if [ $platform = "unknown" ]; then
		echo "Not support QPA platform :${PN}"
		return
	fi

	install -d ${D}${systemd_unitdir}/system-generators
	install -d ${D}${sysconfdir}/qboot
	install -m 0755 ${S}/nx_qtbootlauncher ${D}${systemd_unitdir}/system-generators/

	if [ "${QT_BOOTLAUNCHER_TSLIB}" != "no_tslib" ]; then
		install -m 0755 ${S}/launcher-${platform}-${QT_BOOTLAUNCHER_TSLIB}-${QT_BOOTLAUNCHER_TSLIB_INV}.conf ${output}
	else
		install -m 0755 ${S}/launcher-${platform}-${QT_BOOTLAUNCHER_TSLIB}.conf ${output}
	fi

	if [ ! -z "${TSDEVICE}" ]; then
		sed -i -e 's,TSLIB_TSDEVICE=.*,TSLIB_TSDEVICE=${TSDEVICE},g' ${output}
	fi

	if [ ! -z "${EVDEV_TOUCHSCREEN_PARAMETERS}" ]; then
		sed -i -e 's,QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=.*,QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=${EVDEV_TOUCHSCREEN_PARAMETERS},g' ${output}
	fi

	# set qt platform : BOOT_LAUNCHER_ARGS, QT_QPA_PLATFORM
	if [ "${QT_BOOTLAUNCHER_PLATFORM}" = "directfb" ]; then
		sed -i -e "s/^BOOT_LAUNCHER_ARGS.*/BOOT_LAUNCHER_ARGS=\"-platform ${QT_BOOTLAUNCHER_PLATFORM}\"/" ${output}
		sed -i -e "s/^QT_QPA_PLATFORM.*/QT_QPA_PLATFORM=${QT_BOOTLAUNCHER_PLATFORM}/" ${output}
	fi
}

FILES_${PN} = "${systemd_unitdir}/system-generators ${sysconfdir}/qboot"
