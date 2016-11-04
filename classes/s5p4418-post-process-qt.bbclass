postprocess_about_qt_touch_function() {
    cd ${IMAGE_ROOTFS}
    #echo "export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=\"rotate=180\":tslib:/dev/input/touchscreen0:invertx" >> etc/profile.d/tslib.sh
    
    #echo "export QT_PLUGIN_PATH=/usr/lib/qt5/plugins" >> etc/profile.d/tslib.sh
    #echo "export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/plugins/platforms" >> etc/profile.d/tslib.sh
    echo "export TSLIB_CALIBFILE=/etc/pointercal" >> etc/profile.d/tslib.sh
    echo "export POINTERCAL_FILE=/etc/pointercal" >> etc/profile.d/tslib.sh
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_about_qt_touch_function;"
