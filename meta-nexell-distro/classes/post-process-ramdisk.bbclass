postprocess_ramdisk_function() {
    cd ${IMAGE_ROOTFS}
    echo "ln -sf bin/busybox linuxrc" >> ./init
    echo "rm -rf run" >> ./init
    echo "ln -sf tmp run" >> ./init

    sed -i "/exec sh/d" ./init

    echo "mount -t devtmpfs none /dev" >> ./init
    echo "mount -t proc none /proc" >> ./init
    echo "mount -t sysfs none /sys" >> ./init

    echo "exec sh" >> ./init
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_ramdisk_function;"