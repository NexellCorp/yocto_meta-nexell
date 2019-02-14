inherit linux-nexell-base

DEFAULT_QT_PLATFORM ?= "${@bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-eglfs', 'EGLFS', \
						bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-wayland', 'WAYLAND', '', d), d)}"
TSLIB_ENABLED ?= "${@bb.utils.contains('DISTRO_FEATURES', 'no-use-tslib', 'false', 'true', d)}"

postprocess_qt_function() {
	cd ${IMAGE_ROOTFS}

	if [ "${DEFAULT_QT_PLATFORM}" = "EGLFS" ]; then
		echo "export QT_QPA_PLATFORM=eglfs" >> usr/bin/nexell-qt5-touchsetup.sh
		echo "export QT_QPA_EGLFS_KMS_CONFIG=/etc/qboot/eglfs_config.json" >> usr/bin/nexell-qt5-touchsetup.sh
		echo "export QT_QPA_EGLFS_SWAPINTERVAL=0" >> usr/bin/nexell-qt5-touchsetup.sh
		echo "export QT_QPA_EGLFS_HIDECURSOR=0" >> usr/bin/nexell-qt5-touchsetup.sh
		echo "export QT_QPA_EGLFS_DEBUG=0" >> usr/bin/nexell-qt5-touchsetup.sh
		if [ "${TSLIB_ENABLED}" = "false" ]; then
			echo "export QT_QPA_EGLFS_NO_LIBINPUT=0" >> usr/bin/nexell-qt5-touchsetup.sh
			echo "export QT_QPA_EGLFS_TSLIB=0" >> usr/bin/nexell-qt5-touchsetup.sh
		else
			echo "export QT_QPA_EGLFS_NO_LIBINPUT=1" >> usr/bin/nexell-qt5-touchsetup.sh
			echo "export QT_QPA_EGLFS_TSLIB=1" >> usr/bin/nexell-qt5-touchsetup.sh
		fi
	else
		echo "export QT_QPA_PLATFORM=wayland" >> usr/bin/nexell-qt5-touchsetup.sh
	fi

	if [ "${TSLIB_ENABLED}" = "false" ]; then
		echo "export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=/dev/input/touchscreen0" >> usr/bin/nexell-qt5-touchsetup.sh
	else
		echo "export TSLIB_CALIBFILE=/etc/pointercal" >> usr/bin/nexell-qt5-touchsetup.sh
		echo "export POINTERCAL_FILE=/etc/pointercal" >> usr/bin/nexell-qt5-touchsetup.sh
		echo "export TSLIB_PLUGINDIR=/usr/lib/ts" >> usr/bin/nexell-qt5-touchsetup.sh
		echo "export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=tslib:/dev/input/touchscreen0:invertx:rotate=180" >> usr/bin/nexell-qt5-touchsetup.sh
	fi

	echo "export QT_PLUGIN_PATH=/usr/lib/qt5/plugins" >> usr/bin/nexell-qt5-touchsetup.sh
	echo "export QT_QPA_FONTDIR=/usr/share/fonts/ttf" >> usr/bin/nexell-qt5-touchsetup.sh
	echo "export QT_LOGGING_RULES=qt.qpa.*=false" >> usr/bin/nexell-qt5-touchsetup.sh

	cd ${IMAGE_ROOTFS}/usr/lib
	touch libMali_for_${DEFAULT_QT_PLATFORM}

	# automount iocharset modified
	cd ${IMAGE_ROOTFS}
	cp ${BASE_WORKDIR}/use-post-process/mount.sh etc/udev/scripts/

	# alsa-restore.service & alsa-state.service remove
#	rm ./lib/systemd/system/alsa-restore.service
#	rm ./lib/systemd/system/alsa-state.service
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_qt_function;"
