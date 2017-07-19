mali_postprocess_function() {
    # cd ${IMAGE_ROOTFS}/usr/lib/driver
    # ln -sf libMali.so libEGL.so.1.4; ln -sf libEGL.so.1.4 libEGL.so.1; ln -sf libEGL.so.1 libEGL.so; \
    # ln -sf libMali.so libGLESv1_CM.so.1.1; ln -sf libGLESv1_CM.so.1.1 libGLESv1_CM.so.1; ln -sf libGLESv1_CM.so.1 libGLESv1_CM.so; \
    # ln -sf libMali.so libGLESv2.so.2.0; ln -sf libGLESv2.so.2.0 libGLESv2.so.2; ln -sf libGLESv2.so.2 libGLESv2.so; \

    # ln -sf libMali.so libgbm.so.1; ln -sf libgbm.so.1 libgbm.so; \
    # ln -sf libMali.so libwayland-egl.so.1; ln -sf libwayland-egl.so.1 libwayland-egl.so; \

    # cd ${IMAGE_ROOTFS}/usr/lib
    # ln -sf libGLESv1_CM.so.1.1  libGLESv1_CM.so.1
    # ln -sf driver/libGLESv1_CM.so.1.1 libGLESv1_CM.so.1.1

    cd ${IMAGE_ROOTFS}/usr/bin
    chmod +x start_wayland.sh

    cd ${IMAGE_ROOTFS}
    echo "/usr/lib/driver" > etc/ld.so.conf
    mkdir -p etc/ld.so.conf.d
    echo "/usr/lib/driver" > etc/ld.so.conf.d/mali.conf

    cd ${IMAGE_ROOTFS}/etc/init.d
    echo "#!/bin/sh" > nexell_mali
    echo "/sbin/ldconfig > /dev/null 2>&1" >> nexell_mali
    echo "export DBUS_SESSION_BUS_ADDRESS='unix:path=/var/run/dbus/system_bus_socket'" >> nexell_mali

    chmod +x nexell_mali

    cd ${IMAGE_ROOTFS}/etc/rc2.d
    ln -s ../init.d/weston S11nexell_mali
}

#ROOTFS_POSTPROCESS_COMMAND += "mali_postprocess_function;"
