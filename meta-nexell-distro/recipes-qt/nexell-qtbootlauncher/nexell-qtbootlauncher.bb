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

def convert_to_upper_string(d, s):
    upper_string = s.upper()
    return upper_string

PLATFORM_TYPE = "${@bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-eglfs', 'eglfs', \
				bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-linuxfb', 'linuxfb', 'not_supported', d), d)}"

USECASE_TSLIB = "${@bb.utils.contains('DISTRO_FEATURES', 'no-use-tslib', 'no_tslib', 'tslib', d)}"
INVERTX_TSLIB = "${@bb.utils.contains('DISTRO_FEATURES', 'no-invertx-tslib', 'no_invertx', 'invertx', d)}"

TSDEVICE ?= ""
EVDEV_TOUCHSCREEN_PARAMETERS ?= ""

BOOTTIME_LAUNCHER ?= "0"

do_compile() {
	oe_runmake CROSS_COMPILE="${TARGET_PREFIX}" CC="${CC}" CFLAGS="${CFLAGS} -D${@convert_to_upper_string(d, "${PLATFORM_TYPE}")} -D${@convert_to_upper_string(d, "${USECASE_TSLIB}")} -DBOOTTIME_LAUNCHER=${BOOTTIME_LAUNCHER}"
}

do_install() {
	install -d ${D}${systemd_unitdir}/system-generators
	install -d ${D}${sysconfdir}/qboot

	install -m 0755 ${S}/nx_qtbootlauncher ${D}${systemd_unitdir}/system-generators/

	if [ "${PLATFORM_TYPE}" != "not_supported" ]; then
		if [ "${USECASE_TSLIB}" != "no_tslib" ]; then
			install -m 0755 ${S}/launcher-${PLATFORM_TYPE}-${USECASE_TSLIB}-${INVERTX_TSLIB}.conf ${D}${sysconfdir}/qboot/launcher.conf
		else
			install -m 0755 ${S}/launcher-${PLATFORM_TYPE}-${USECASE_TSLIB}.conf ${D}${sysconfdir}/qboot/launcher.conf
		fi

		if [ ! -z "${TSDEVICE}" ]; then
			sed -i -e 's,TSLIB_TSDEVICE=.*,TSLIB_TSDEVICE=${TSDEVICE},g' ${D}${sysconfdir}/qboot/launcher.conf
		fi

		if [ ! -z "${EVDEV_TOUCHSCREEN_PARAMETERS}" ]; then
			sed -i -e 's,QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=.*,QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=${EVDEV_TOUCHSCREEN_PARAMETERS},g' ${D}${sysconfdir}/qboot/launcher.conf
		fi
	else
		echo "Can't find supported QPA platform."
	fi
}

FILES_${PN} = "${systemd_unitdir}/system-generators ${sysconfdir}/qboot"
