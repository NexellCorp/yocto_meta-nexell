# Base this image on nexell-hwup-image
#include nexell-hwup-image.bb
include nexell-image-qt5.bb

LICENSE = "GPLv2"
SPLASH = "psplash-nexell"

#IMAGE_FEATURES += "ssh-server-dropbear splash"
