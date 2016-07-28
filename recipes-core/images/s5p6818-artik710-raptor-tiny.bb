# Base this image on nexell-image-minimal
include nexell-image-tiny.inc
inherit s5p6818-artik710-raptor-post-process

LICENSE = "LGPLv2.1"

IMAGE_INSTALL += " \
               optee-linuxdriver \
               optee-build \
               "
