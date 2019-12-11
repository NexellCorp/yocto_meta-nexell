inherit core-image
inherit nexell-rootfs
inherit nexell-image

PACKAGE_INSTALL = " \
    initramfs-boot \
    busybox \
    bash \
    ${ROOTFS_BOOTSTRAP_INSTALL} \
    "

# Do not pollute the initrd image with rootfs features
IMAGE_FEATURES = ""

export IMAGE_BASENAME = "nexell-image-tiny"
IMAGE_LINGUAS = ""

LICENSE = "MIT"

inherit core-image

IMAGE_ROOTFS_SIZE ?= "8192"
IMAGE_ROOTFS_EXTRA_SPACE ?= "0"
BAD_RECOMMENDATIONS += "busybox-syslog"
