# Base code : meta/recipes-core/images/core-image-tiny-initramfs.bb

PACKAGE_INSTALL = "initramfs-boot busybox bash base-files udev ${ROOTFS_BOOTSTRAP_INSTALL}"

IMAGE_FEATURES = ""

export IMAGE_BASENAME = "core-image-tiny-initramfs"
IMAGE_LINGUAS = ""

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

inherit core-image

IMAGE_ROOTFS_SIZE = "16384"
IMAGE_ROOTFS_EXTRA_SPACE = "0"

BAD_RECOMMENDATIONS += "busybox-syslog"

# NOTE we must use cpio.gz here as this is what mkbootimg requires
IMAGE_FSTYPES_forcevariable = "cpio.gz"

# USER customize
PACKAGE_INSTALL += ""