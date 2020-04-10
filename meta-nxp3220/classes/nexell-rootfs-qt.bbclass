# nexell rootfs for Qt

QT_TOUCHSETUP_PLATFORM = \
	"${@bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-eglfs', 'eglfs', \
            bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-linuxfb', 'linuxfb', \
	    bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-directfb', 'directfb', 'not_supported', \
            d), d), d)}"

postprocess_qt_function() {

	cd ${IMAGE_ROOTFS}

	output="usr/bin/nexell-qt5-touchsetup.sh"
	platform="${QT_TOUCHSETUP_PLATFORM}"
	[ "${QT_TOUCHSETUP_PLATFORM}" = "linuxfb" ] && platform="linuxfb:fb=/dev/fb0:nographicsmodeswitch";

	echo "export QT_QPA_PLATFORM=${platform}" >> ${output}
	echo "export QT_PLUGIN_PATH=/usr/lib/qt5/plugins" >> ${output}
	echo "export QT_QPA_FONTDIR=/usr/share/fonts/ttf" >> ${output}
	echo "export QT_LOGGING_RULES=qt.qpa.*=false" >> ${output}

	if ${@bb.utils.contains('DISTRO_FEATURES', 'no-use-tslib', 'true', 'false', d)}; then
		echo "export QT_QPA_FB_TSLIB=0" >> ${output}
		echo "export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=/dev/input/touchscreen0" >> ${output}
	else
		echo "export QT_QPA_FB_NO_LIBINPUT=1" >> ${output}
		echo "export QT_QPA_FB_TSLIB=1" >> ${output}

		if ${@bb.utils.contains('DISTRO_FEATURES', 'no-invertx-tslib', 'true', 'false', d)}; then
			echo "export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=tslib:/dev/input/touchscreen0" >> ${output}
		else
			echo "export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=tslib:/dev/input/touchscreen0:invertx:rotate=180" >> ${output}
		fi

		echo "export TSLIB_CALIBFILE=/etc/pointercal" >> ${output}
		echo "export TSLIB_PLUGINDIR=/usr/lib/ts" >> ${output}
		echo "export POINTERCAL_FILE=/etc/pointercal" >> ${output}
	fi

	# disable auto mount rules
	if ${@bb.utils.contains('DISTRO_FEATURES', 'no-use-automount', 'true', 'false', d)}; then
		rm etc/udev/rules.d/local.rules
		rm etc/udev/rules.d/automount.rules
	fi
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_qt_function;"
