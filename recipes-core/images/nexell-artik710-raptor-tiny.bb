# Base this image on nexell-image-minimal
include nexell-image-tiny.bb

LICENSE = "LGPLv2.1"
SPLASH = "psplash-nexell"

IMAGE_FEATURES += "ssh-server-dropbear splash"
