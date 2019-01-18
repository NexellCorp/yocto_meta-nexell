TSLIB_ENABLED ?= "${@bb.utils.contains('DISTRO_FEATURES', 'no-use-tslib', 'false', 'true', d)}"

postprocess_qt_function() {
	cd ${IMAGE_ROOTFS}

	echo "export QT_QPA_PLATFORM=linuxfb:fb=/dev/fb0:nographicsmodeswitch" >> usr/bin/nexell-qt5-touchsetup.sh

	if [ "${TSLIB_ENABLED}" = "false" ]; then
		echo "export QT_QPA_FB_TSLIB=0" >> usr/bin/nexell-qt5-touchsetup.sh
		echo "export QT_QPA_FB_NO_LIBINPUT=0" >> usr/bin/nexell-qt5-touchsetup.sh
		echo "export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=/dev/input/touchscreen0" >> usr/bin/nexell-qt5-touchsetup.sh
	else
		echo "export QT_QPA_FB_TSLIB=1" >> usr/bin/nexell-qt5-touchsetup.sh
		echo "export QT_QPA_FB_NO_LIBINPUT=1" >> usr/bin/nexell-qt5-touchsetup.sh
		echo "export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=tslib:/dev/input/touchscreen0:invertx:rotate=180" >> usr/bin/nexell-qt5-touchsetup.sh
		echo "export TSLIB_CALIBFILE=/etc/pointercal" >> usr/bin/nexell-qt5-touchsetup.sh
		echo "export TSLIB_PLUGINDIR=/usr/lib/ts" >> usr/bin/nexell-qt5-touchsetup.sh
		echo "export POINTERCAL_FILE=/etc/pointercal" >> usr/bin/nexell-qt5-touchsetup.sh
	fi

	echo "export QT_PLUGIN_PATH=/usr/lib/qt5/plugins" >> usr/bin/nexell-qt5-touchsetup.sh
	echo "export QT_QPA_FONTDIR=/usr/share/fonts/ttf" >> usr/bin/nexell-qt5-touchsetup.sh
	echo "export QT_LOGGING_RULES=qt.qpa.*=false" >> usr/bin/nexell-qt5-touchsetup.sh
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_qt_function;"
