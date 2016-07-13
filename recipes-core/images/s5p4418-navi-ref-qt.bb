# Base this image on nexell-image-qt5
require recipes-graphics/images/core-image-weston.bb
include nexell-image-qt5.inc
inherit s5p4418-navi-ref-post-process
inherit s5p4418-navi-ref-post-process-qt

LICENSE = "LGPLv2.1"

IMAGE_INSTALL_append = "packagegroup-s5p4418-navi-ref-qt"