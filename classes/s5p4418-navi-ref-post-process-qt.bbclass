navi_postprocess_about_qt_touch_function() {
    cd ${IMAGE_ROOTFS}
    echo "export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=\"rotate=180\":tslib:/dev/input/touchscreen0:invertx" >> etc/profile.d/tslib.sh
    
    echo "export QT_PLUGIN_PATH=/usr/lib/qt5/plugins" >> etc/profile.d/tslib.sh
    echo "export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/plugins/platforms" >> etc/profile.d/tslib.sh
    echo "export TSLIB_CALIBFILE=/etc/pointercal" >> etc/profile.d/tslib.sh
    echo "export POINTERCAL_FILE=/etc/pointercal" >> etc/profile.d/tslib.sh

    echo "17823 -130 -3379408 -124 -10543 42136912 65536 1024 600" > etc/pointercal
}

navi_postprocess_mali_function() {
    cd ${IMAGE_ROOTFS}/usr/lib/driver
    ln -sf libMali.so libEGL.so.1.4; ln -sf libEGL.so.1.4 libEGL.so.1; ln -sf libEGL.so.1 libEGL.so; \
    ln -sf libMali.so libGLESv1_CM.so.1.1; ln -sf libGLESv1_CM.so.1.1 libGLESv1_CM.so.1; ln -sf libGLESv1_CM.so.1 libGLESv1_CM.so; \
    ln -sf libMali.so libGLESv2.so.2.0; ln -sf libGLESv2.so.2.0 libGLESv2.so.2; ln -sf libGLESv2.so.2 libGLESv2.so; \

    ln -sf libMali.so libgbm.so.1; ln -sf libgbm.so.1 libgbm.so; \
    ln -sf libMali.so libwayland-egl.so.1; ln -sf libwayland-egl.so.1 libwayland-egl.so; \

    cd ${IMAGE_ROOTFS}/usr/lib
    ln -sf libGLESv1_CM.so.1.1  libGLESv1_CM.so.1
    ln -sf driver/libGLESv1_CM.so.1.1 libGLESv1_CM.so.1.1

    cd ${IMAGE_ROOTFS}
    echo "/usr/lib/driver" > etc/ld.so.conf
    mkdir -p etc/ld.so.conf.d
    echo "/usr/lib/driver" > etc/ld.so.conf.d/mali.conf

    cd ${IMAGE_ROOTFS}/etc/init.d
    echo "#!/bin/sh" > nexell_mali
    echo "/sbin/ldconfig > /dev/null 2>&1" >> nexell_mali
    chmod +x nexell_mali
    
    cd ${IMAGE_ROOTFS}/etc/rc2.d
    ln -s ../init.d/weston S11nexell_mali
    
}

ROOTFS_POSTPROCESS_COMMAND += "navi_postprocess_about_qt_touch_function;navi_postprocess_mali_function"
