# Base this image on nexell-image-qt5
include nexell-image-tinyui.inc
inherit s5p4418-navi-ref-post-process
inherit nexell-mali-postprocess

LICENSE = "LGPLv2.1"

IMAGE_INSTALL_append = "packagegroup-s5p4418-navi-ref-tinyui"