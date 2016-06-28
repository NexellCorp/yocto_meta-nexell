# Base this image on nexell-image-minimal
include nexell-image-tiny.inc
inherit nexell-post-process-artik710-raptor

LICENSE = "LGPLv2.1"
SPLASH = "psplash-nexell"

IMAGE_FEATURES += "ssh-server-dropbear splash"
