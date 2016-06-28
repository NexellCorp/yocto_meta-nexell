# Base this image on nexell-image-sato
include nexell-image-sato.inc
inherit nexell-post-process-artik710-raptor

LICENSE = "LGPLv2.1"
SPLASH = "psplash-nexell"

IMAGE_FEATURES += "ssh-server-dropbear splash"
