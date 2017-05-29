postprocess_about_sdl_touch_function() {
    cd ${IMAGE_ROOTFS}
    echo "export TSLIB_CALIBFILE=/etc/pointercal" >> etc/profile.d/tslib.sh
    echo "export POINTERCAL_FILE=/etc/pointercal" >> etc/profile.d/tslib.sh

    #weston/wayland
    echo "SUBSYSTEM==\"input\", ENV{WL_CALIBRATION}=\"1.099785 0.007999 -55.025471 -0.000000 -1.090909 618.545471\"" >> etc/udev/rules.d/touchscreen.rules

    #adb configfs mount bug fix
    sed -i "/mount -t configfs none \/sys\/kernel\/config/d" usr/bin/start_adbd.sh

}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_about_sdl_touch_function;"
