postprocess_about_sato_touch_function() {
    cd ${IMAGE_ROOTFS}

    #X11 touch pointer default calibration data do not make
    rm etc/X11/Xsession.d/30xinput_calibrate.sh
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_about_sato_touch_function;"
