# Base this image on nexell-image-qt5
require ../nexell-image-sdl.inc
inherit s5p4418-post-process
inherit s5p4418-post-process-sdl

LICENSE = "LGPLv2.1"

IMAGE_INSTALL += " \
    android-tools \
    packagegroup-s5p4418-sdl \
    "
