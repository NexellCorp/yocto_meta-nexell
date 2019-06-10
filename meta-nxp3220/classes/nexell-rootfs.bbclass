# rootfs commands to implement root file system

postprocess_rootfs() {
    cd ${IMAGE_ROOTFS}

    ln -sf bin/busybox linuxrc

    # ---bootscript.sh create---
    echo "#!/bin/sh" > ./etc/bootscript.sh
    echo "mount -t devtmpfs none /dev" >> etc/bootscript.sh
    echo "mount -t proc none /proc" >> etc/bootscript.sh
    echo "mount -t sysfs none /sys" >> etc/bootscript.sh

    # ping permission change
    echo "inet:x:3003:root"    >> etc/group
    echo "net_raw:x:3004:root" >> etc/group

    # data partition add to fstab for ext4
    if ${@bb.utils.contains('IMAGE_FSTYPES','ext4','true','false',d)}; then
	if [ ! -z "${PART_DATA_SIZE}" ]; then
            echo "/dev/mmcblk0p4 /data ext4 noatime,nosuid,nodev,nomblk_io_submit,errors=panic wait,check" >> etc/fstab
        fi
    fi

    # data partition add to fstab for ubi
    if ${@bb.utils.contains('IMAGE_FSTYPES','multiubi2','true','false',d)}; then
	if [ ! -z "${MKUBIFS_ARGS_data}" ]; then
	    echo "ubi1:userdata /data ubifs defaults,auto 0 0" >> etc/fstab
        fi
    fi
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_rootfs;"
