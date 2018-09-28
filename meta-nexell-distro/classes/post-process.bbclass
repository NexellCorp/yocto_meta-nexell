postprocess_common_function() {
    cd ${IMAGE_ROOTFS}
    ln -sf bin/busybox linuxrc
    rm -rf run
    ln -sf tmp run

    #---bootscript.sh create---
    echo "#!/bin/sh" > ./etc/bootscript.sh
    echo "mount -t devtmpfs none /dev" >> etc/bootscript.sh
    echo "mount -t proc none /proc" >> etc/bootscript.sh
    echo "mount -t sysfs none /sys" >> etc/bootscript.sh

    #ping permission change
    echo "inet:x:3003:root"    >> etc/group
    echo "net_raw:x:3004:root" >> etc/group

    # data partition add
    echo "/dev/mmcblk0p4 /data         ext4     noatime,nosuid,nodev,nomblk_io_submit,errors=panic wait,check" >> etc/fstab
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_common_function;"