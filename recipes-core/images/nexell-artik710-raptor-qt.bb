# Base this image on nexell-image-qt5
require recipes-graphics/images/core-image-weston.bb
inherit nexell-post-process-artik710-raptor
include nexell-image-qt5.inc

LICENSE = "LGPLv2.1"

IMAGE_INSTALL += " \
               optee-linuxdriver \
               optee-build \
               "
