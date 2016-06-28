# Base this image on core-image-minimal
include recipes-core/images/core-image-minimal.bb
#inherit core-image

LICENSE = "GPLv2"

# Include modules in rootfs
IMAGE_INSTALL += " \
    kernel-modules \
    "

IMAGE_INSTALL += "packagegroup-nexell-tinygui"
#IMAGE_INSTALL_append = " mali-nexell clutter-example mx-tests psplash-nexell"
IMAGE_INSTALL_append = " mali-nexell psplash-nexell"
IMAGE_INSTALL_append += " libstdc++"