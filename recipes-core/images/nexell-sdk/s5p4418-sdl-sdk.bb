# Base this image on nexell-image-minimal
require ../nexell-image-sdl.inc

LICENSE = "LGPLv2.1"

IMAGE_INSTALL += " \
    packagegroup-nexell-sdk \
    "