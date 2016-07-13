# Base this image on nexell-image-sato
include nexell-image-sato.inc
inherit s5p6818-artik710-raptor-post-process

LICENSE = "LGPLv2.1"
IMAGE_INSTALL += " \
               optee-linuxdriver \
               optee-build \
               "
