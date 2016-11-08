# Base this image on nexell-image-minimal
require ../nexell-image-tiny.inc
require ../nexell-image-common.inc
inherit s5p6818-post-process

LICENSE = "LGPLv2.1"

IMAGE_INSTALL += " \
    optee-linuxdriver \
    optee-build \
    "
