# Base this image on nexell-image-minimal
include nexell-image-tiny.inc
inherit nexell-post-process-artik710-raptor

LICENSE = "LGPLv2.1"

IMAGE_INSTALL += " \
               optee-linuxdriver \
               optee-build \
               "
