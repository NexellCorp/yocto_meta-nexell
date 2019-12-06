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

    # data/misc partition add to fstab for ext4
    if ${@bb.utils.contains('IMAGE_FSTYPES','ext4','true','false',d)}; then
	if [ ! -z "${PART_DATA_SIZE}" ] && [ ! -z "${PART_DATA_NODE}" ]; then
            echo "${PART_DATA_NODE} /data ext4 noatime,nosuid,nodev,nomblk_io_submit,errors=panic wait,check" >> etc/fstab
        fi
	if [ ! -z "${PART_MISC_SIZE}" ] && [ ! -z "${PART_MISC_NODE}" ]; then
            echo "${PART_MISC_NODE} /misc ext4 noatime,nosuid,nodev,nomblk_io_submit,errors=panic wait,check" >> etc/fstab
        fi
    fi

    # data/misc partition add to fstab for ubi
    if ${@bb.utils.contains('IMAGE_FSTYPES','multiubi2','true','false',d)}; then
	if [ ! -z "${MKUBIFS_ARGS_data}" ] && [ ! -z "${PART_DATA_NODE}" ]; then
	    echo "${PART_DATA_NODE} /data ubifs defaults,auto 0 0" >> etc/fstab
        fi
	if [ ! -z "${MKUBIFS_ARGS_misc}" ] && [ ! -z "${PART_MISC_NODE}" ]; then
	    echo "${PART_MISC_NODE} /misc ubifs defaults,auto 0 0" >> etc/fstab
        fi
    fi

    # remove include directories
    #
    find ${IMAGE_ROOTFS} -name include | xargs rm -rf
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_rootfs;"
