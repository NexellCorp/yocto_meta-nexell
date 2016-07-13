# Base this image on nexell-image-qt5
require recipes-graphics/images/core-image-weston.bb
include nexell-image-qt5.inc
inherit s5p6818-artik710-raptor-post-process
inherit s5p6818-artik710-raptor-post-process-qt

LICENSE = "LGPLv2.1"

#IMAGE_INSTALL += " \
#               optee-linuxdriver \
#               optee-build \
#               "

IMAGE_INSTALL_append = "packagegroup-s5p6818-artik710-raptor-qt"