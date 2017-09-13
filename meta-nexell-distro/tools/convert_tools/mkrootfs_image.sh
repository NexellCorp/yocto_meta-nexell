#!/bin/bash

#set -e
# 1 target path ==> out/result path
# 2 nexell-xxx-xxx.ext4 for convert sparse
# 3 seek size
# 4 support

if [ "$TARGET_DIR" == "" ]; then
    TARGET_DIR=$1
fi

OUTPUT_NAME=$2
ROOTFS_SIZE_M=$3
EXTRA_ROOTFS=$4

dd if=/dev/zero of=$TARGET_DIR/${OUTPUT_NAME} seek=${ROOTFS_SIZE_M} bs=1M count=0

mkdir -p $TARGET_DIR/rootfs
mkfs.ext4 -F -b 4096 -L rootfs $TARGET_DIR/${OUTPUT_NAME}
sudo mount -o loop $TARGET_DIR/${OUTPUT_NAME} $TARGET_DIR/rootfs
sudo tar xvzfp $TARGET_DIR/rootfs.tar.gz -C $TARGET_DIR/rootfs
sudo cp -rp ${EXTRA_ROOTFS}/* $TARGET_DIR/rootfs/

sync

sudo umount $TARGET_DIR/rootfs
e2fsck -y -f $TARGET_DIR/${OUTPUT_NAME}
sudo rm -rf $TARGET_DIR/rootfs
