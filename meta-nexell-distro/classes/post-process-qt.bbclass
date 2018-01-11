inherit linux-nexell-base

DEFAULT_QT_PLATFORM ?= "${@bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-eglfs', 'FB', \
                           bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-wayland', 'WAYLAND', '', d), d)}"

postprocess_qt_function() {
    cd ${IMAGE_ROOTFS}

    if [ "${DEFAULT_QT_PLATFORM}" = "FB" ]; then
        #EGLFS or linuxFB
        echo "export QT_QPA_PLATFORM=eglfs" >> usr/bin/nexell-qt5-touchsetup.sh
        echo "export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=tslib:/dev/input/touchscreen0:invertx:rotate=180" >> usr/bin/nexell-qt5-touchsetup.sh
        #echo "export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/plugins/platforms" >> etc/profile.d/tslib.sh
    else
        #weston/wayland
        #echo "SUBSYSTEM==\"input\", ENV{WL_CALIBRATION}=\"1.099785 0.007999 -55.025471 -0.000000 -1.090909 618.545471\"" >> etc/udev/rules.d/touchscreen.rules
        echo "export QT_QPA_PLATFORM=wayland" >> usr/bin/nexell-qt5-touchsetup.sh
    fi

    echo "export QT_PLUGIN_PATH=/usr/lib/qt5/plugins" >> usr/bin/nexell-qt5-touchsetup.sh
    echo "export TSLIB_CALIBFILE=/etc/pointercal" >> usr/bin/nexell-qt5-touchsetup.sh
    echo "export POINTERCAL_FILE=/etc/pointercal" >> usr/bin/nexell-qt5-touchsetup.sh


    if [ "${NEXELL_TOUCH_CLASS}" = "CAPACITIVE" ]; then
        echo "export QT_QPA_EGLFS_NO_LIBINPUT=0" >> usr/bin/nexell-qt5-touchsetup.sh
        echo "export QT_QPA_EGLFS_TSLIB=0" >> usr/bin/nexell-qt5-touchsetup.sh
    else
        echo "export QT_QPA_EGLFS_NO_LIBINPUT=1" >> usr/bin/nexell-qt5-touchsetup.sh
        echo "export QT_QPA_EGLFS_TSLIB=1" >> usr/bin/nexell-qt5-touchsetup.sh
    fi

    #adbd do not start on boot time, If you need use adbd, then you can  run /usr/bin/start_adbd.sh
    cd ${IMAGE_ROOTFS}/lib/systemd/system
    sed -i 's/ExecStart=\/usr\/bin\/adbd/\#ExecStart=\/usr\/bin\/adbd/g' android-tools-adbd.service

    cd ${IMAGE_ROOTFS}/usr/lib
    touch libMali_for_${DEFAULT_QT_PLATFORM}
    if [ "${ARCH_TYPE_NUM}" -eq "64" ]; then
        # 64bit FB library does not support, TBD
        ln -sf ./nexell/libMali-${ARCH_TYPE_NUM}-WAYLAND libMali.so
    else
        ln -sf ./nexell/libMali-${ARCH_TYPE_NUM}-${DEFAULT_QT_PLATFORM}.so libMali.so
    fi

    #automount iocharset modified
    cd ${IMAGE_ROOTFS}
    cp ${BASE_WORKDIR}/use-post-process/mount.sh etc/udev/scripts/
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_qt_function;"
