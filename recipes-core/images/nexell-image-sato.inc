# Base this image on core-image-sato
include recipes-sato/images/core-image-sato.bb
LICENSE = "GPLv2"

SPLASH = "psplash-nexell"

IMAGE_FEATURES += "package-management ssh-server-dropbear tools-debug splash"

# Include modules in rootfs
IMAGE_INSTALL += " \
        kernel-modules \
	"
