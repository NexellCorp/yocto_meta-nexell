# Base this image on nexell-image-tiny
include nexell-image-tiny.inc
inherit nexell-post-process-artik710-raptor

LICENSE = "CLOSED"

IMAGE_INSTALL += " \
               optee-linuxdriver \
               optee-build \
               "
