# Base this image on nexell-image-sato
include nexell-image-sato.bb

LICENSE = "LGPLv2.1"
SPLASH = "psplash-nexell"

IMAGE_FEATURES += "ssh-server-dropbear splash"
