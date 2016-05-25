artik7_postprocess_function() {
    cd ${IMAGE_ROOTFS}    
    ln -sf bin/busybox linuxrc
    ln -sf lib lib64
    rm -rf run
    ln -sf tmp run
    echo "meta-nexell artik710 QT build" > ${IMAGE_ROOTFS}/nexell-artik710-raptor-qt-timeStamp.txt

    #---bootscript.sh create---
    echo "#!/bin/sh" > ./etc/bootscript.sh
    echo "mount -t devtmpfs none /dev" >> etc/bootscript.sh
    echo "mount -t proc none /proc" >> etc/bootscript.sh
    echo "mount -t sysfs none /sys" >> etc/bootscript.sh
}

ROOTFS_POSTPROCESS_COMMAND += "artik7_postprocess_function; "