# Base this image on nexell-image-tiny
include nexell-image-tiny.inc
inherit nexell-post-process-artik710-raptor

LICENSE = "CLOSED"
SPLASH = "psplash-nexell"

IMAGE_FEATURES += "ssh-server-dropbear splash"

IMAGE_ROOTFS_SIZE ?= "8192"