postprocess_about_qt_touch_function() {
    cd ${IMAGE_ROOTFS}
    #EGLFS or linuxFB
    #echo "export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=\"rotate=180\":tslib:/dev/input/touchscreen0:invertx" >> etc/profile.d/tslib.sh
    
    #echo "export QT_PLUGIN_PATH=/usr/lib/qt5/plugins" >> etc/profile.d/tslib.sh
    #echo "export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/plugins/platforms" >> etc/profile.d/tslib.sh
    echo "export TSLIB_CALIBFILE=/etc/pointercal" >> etc/profile.d/tslib.sh
    echo "export POINTERCAL_FILE=/etc/pointercal" >> etc/profile.d/tslib.sh

    #weston/wayland
    echo "SUBSYSTEM==\"input\", ENV{WL_CALIBRATION}=\"1.099785 0.007999 -55.025471 -0.000000 -1.090909 618.545471\"" >> etc/udev/rules.d/touchscreen.rules

    #adb configfs mount bug fix
    sed -i "/mount -t configfs none \/sys\/kernel\/config/d" usr/bin/start_adbd.sh

    #automount iocharset modified
    cp ${BASE_WORKDIR}/use-post-process/mount.sh etc/udev/scripts/

    #alsa-restore.service & alsa-state.service remove
    rm ./lib/systemd/system/alsa-restore.service
    rm ./lib/systemd/system/alsa-state.service
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_about_qt_touch_function;"
