#inherit post-process-ramdisk

SPALSH = "psplash-nexell"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

#IMAGE_INSTALL = "busybox base-passwd initramfs-boot bash"
IMAGE_FEATURES = ""
IMAGE_ROOTFS_SIZE = "16384"
IMAGE_ROOTFS_EXTRA_SPACE = "0"
export IMAGE_BASENAME = "core-image-minimal-initramfs"

IMAGE_LINGUAS = ""

# NOTE we must use cpio.gz here as this is what mkbootimg requires
IMAGE_FSTYPES_forcevariable = "cpio.gz"

# We don't need depmod data here
KERNELDEPMODDEPEND = ""
USE_DEPMOD = "0"
PACKAGE_INSTALL += "systemd busybox android-tools-nexell e2fsprogs bash psplash util-linux-sfdisk nexell-nxupdate"

inherit core-image
