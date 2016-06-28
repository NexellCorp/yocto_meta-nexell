artik7_make_mali_symlink_function() {
    cd ${IMAGE_ROOTFS}/usr/lib
    
    ln -sf libEGL.so.1 libEGL.so
    ln -sf libEGL.so.1.4 libEGL.so.1
    ln -sf libMali.so libEGL.so.1.4

    ln -sf libGLESv1_CM.so.1 libGLESv1_CM.so
    ln -sf libGLESv1_CM.so.1.1 libGLESv1_CM.so.1
    ln -sf libMali.so libGLESv1_CM.so.1.1
	   
    ln -sf libGLESv2.so.2 libGLESv2.so
    ln -sf libGLESv2.so.2.0 libGLESv2.so.2
    ln -sf libMali.so libGLESv2.so.2.0

    ln -sf libMali.so libgbm.so
    ln -sf libdrm.so.2.4.0 libdrm.so
    ln -sf libdrm.so.2.4.0 libdrm.so.2
}

ROOTFS_POSTPROCESS_COMMAND += " artik7_make_mali_symlink_function; "