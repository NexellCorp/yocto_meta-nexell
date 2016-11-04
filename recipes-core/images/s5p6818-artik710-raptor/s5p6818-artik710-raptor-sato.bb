# Base this image on nexell-image-sato
require ../nexell-image-sato.inc
inherit s5p6818-post-process

LICENSE = "LGPLv2.1"
IMAGE_INSTALL += " \
               optee-linuxdriver \
               optee-build \
               "
